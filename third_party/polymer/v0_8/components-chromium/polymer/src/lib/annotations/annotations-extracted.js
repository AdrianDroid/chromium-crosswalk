
/**
 * Scans a template to produce an annotation list that that associates
 * metadata culled from markup with tree locations 
 * metadata and information to associate the metadata with nodes in an instance.
 *
 * Supported expressions include:
 *
 * Double-mustache annotations in text content. The annotation must be the only
 * content in the tag, compound expressions are not supported.
 *
 *     <[tag]>{{annotation}}<[tag]>
 *
 * Double-escaped annotations in an attribute, either {{}} or [[]].
 *
 *     <[tag] someAttribute="{{annotation}}" another="[[annotation]]"><[tag]>
 *
 * `on-` style event declarations.
 *
 *     <[tag] on-<event-name>="annotation"><[tag]>
 *
 * Note that the `annotations` feature does not implement any behaviors
 * associated with these expressions, it only captures the data.
 *
 * Generated data-structure:
 * 
 *     [
 *       {
 *         id: '<id>',
 *         events: [
 *           {
 *             name: '<name>'
 *             value: '<annotation>'
 *           }, ...
 *         ],
 *         bindings: [
 *           {
 *             kind: ['text'|'attribute'],
 *             mode: ['{'|'['],
 *             name: '<name>'
 *             value: '<annotation>'
 *           }, ...
 *         ],
 *         // TODO(sjmiles): this is annotation-parent, not node-parent
 *         parent: <reference to parent annotation object>,
 *         index: <integer index in parent's childNodes collection>
 *       },
 *       ...
 *     ]
 * 
 * @class Template feature
 */

  // null-array (shared empty array to avoid null-checks)
  Polymer.nar = [];

  Polymer.Annotations = {

    // preprocess-time

    // construct and return a list of annotation records
    // by scanning `template`'s content
    //
    parseAnnotations: function(template) {
      var list = [];
      var content = template._content || template.content; 
      this._parseNodeAnnotations(content, list);
      return list;
    },

    // add annotations gleaned from subtree at `node` to `list`
    _parseNodeAnnotations: function(node, list) {
      return node.nodeType === Node.TEXT_NODE ?
        this._parseTextNodeAnnotation(node, list) :
          // TODO(sjmiles): are there other nodes we may encounter
          // that are not TEXT_NODE but also not ELEMENT?
          this._parseElementAnnotations(node, list);
    },

    // add annotations gleaned from TextNode `node` to `list`
    _parseTextNodeAnnotation: function(node, list) {
      var v = node.textContent, escape = v.slice(0, 2);
      if (escape === '{{' || escape === '[[') {
        // NOTE: use a space here so the textNode remains; some browsers
        // (IE) evacipate an empty textNode.
        node.textContent = ' ';
        var annote = {
          bindings: [{
            kind: 'text',
            mode: escape[0],
            value: v.slice(2, -2)
          }]
        };
        list.push(annote);
        return annote;
      }
    },

    // add annotations gleaned from Element `node` to `list`
    _parseElementAnnotations: function(element, list) {
      var annote = {
        bindings: [],
        events: []
      };
      this._parseChildNodesAnnotations(element, annote, list);
      // TODO(sjmiles): is this for non-ELEMENT nodes? If so, we should
      // change the contract of this method, or filter these out above.
      if (element.attributes) {
        this._parseNodeAttributeAnnotations(element, annote, list);
        // TODO(sorvell): ad hoc callback for doing work on elements while
        // leveraging annotator's tree walk.
        // Consider adding an node callback registry and moving specific 
        // processing out of this module.
        if (this.prepElement) {
          this.prepElement(element);
        }
      }
      if (annote.bindings.length || annote.events.length || annote.id) {
        list.push(annote);
      }
      return annote;
    },

    // add annotations gleaned from children of `root` to `list`, `root`'s
    // `annote` is supplied as it is the annote.parent of added annotations 
    _parseChildNodesAnnotations: function(root, annote, list, callback) {
      if (root.firstChild) {
        for (var i=0, node=root.firstChild; node; node=node.nextSibling, i++){
          if (node.localName === 'template' &&
            !node.hasAttribute('preserve-content')) {
            this._parseTemplate(node, i, list, annote);
          }
          //
          var childAnnotation = this._parseNodeAnnotations(node, list, callback);
          if (childAnnotation) {
            childAnnotation.parent = annote;
            childAnnotation.index = i;
          }
        }
      }
    },

    // 1. Parse annotations from the template and memoize them on
    //    content._notes (recurses into nested templates)
    // 2. Parse template bindings for parent.* properties and memoize them on
    //    content._parentProps
    // 3. Create bindings in current scope's annotation list to template for
    //    parent props found in template
    // 4. Remove template.content and store it in annotation list, where it
    //    will be the responsibility of the host to set it back to the template
    //    (this is both an optimization to avoid re-stamping nested template
    //    children and avoids a bug in Chrome where nested template children
    //    upgrade)
    _parseTemplate: function(node, index, list, parent) {
      // TODO(sjmiles): simply altering the .content reference didn't
      // work (there was some confusion, might need verification)
      var content = document.createDocumentFragment();
      content._notes = this.parseAnnotations(node);
      content.appendChild(node.content);
      // Special-case treatment of 'parent.*' props for nested templates
      // Automatically bind `prop` on host to `_parent_prop` on template
      // for any `parent.prop`'s encountered in template binding; it is
      // responsibility of the template implementation to forward
      // these properties as appropriate
      var bindings = [];
      this._discoverTemplateParentProps(content);
      for (var prop in content._parentProps) {
        bindings.push({
          index: index,
          kind: 'property',
          mode: '{',
          name: '_parent_' + prop,
          value: prop
        });
      }
      // TODO(sjmiles): using `nar` to avoid unnecessary allocation;
      // in general the handling of these arrays needs some cleanup 
      // in this module
      list.push({
        bindings: bindings,
        events: Polymer.nar,
        templateContent: content,
        parent: parent,
        index: index
      });
    },

    // Finds all parent.* properties in template content and stores
    // the path members in content._parentPropChain, which is an array
    // of maps listing the properties of parent templates required at
    // each level. Each outer template merges inner _parentPropChains to
    // propagate inner parent property needs to outer templates.
    // The top-level parent props from the chain (corresponding to this 
    // template) are stored in content._parentProps.
    _discoverTemplateParentProps: function(content) {
      var chain = content._parentPropChain = [];
      content._notes.forEach(function(n) {
        // Find all bindings to parent.* and spread them into _parentPropChain
        n.bindings.forEach(function(b) {
          var m;
          if (m = b.value.match(/parent\.((parent\.)*[^.]*)/)) {
            var parts = m[1].split('.');
            for (var i=0; i<parts.length; i++) {
              var pp = chain[i] || (chain[i] = {});
              pp[parts[i]] = true;
            }
          }
        });
        // Merge child _parentPropChain[n+1] into this _parentPropChain[n]
        if (n.templateContent) {
          var tpp = n.templateContent._parentPropChain;
          for (var i=1; i<tpp.length; i++) {
            if (tpp[i]) {
              var pp = chain[i-1] || (chain[i-1] = {});
              Polymer.Base.simpleMixin(pp, tpp[i]);              
            }
          }
        }
      });
      // Store this template's parentProps map
      content._parentProps = chain[0];
    },

    // add annotation data from attributes to the `annotation` for node `node`
    // TODO(sjmiles): the distinction between an `annotation` and 
    // `annotation data` is not as clear as it could be
    // Walk attributes backwards, since removeAttribute can be vetoed by
    // IE in certain cases (e.g. <input value="foo">), resulting in the
    // attribute staying in the attributes list
    _parseNodeAttributeAnnotations: function(node, annotation) {
      for (var i=node.attributes.length-1, a; (a=node.attributes[i]); i--) {
        var n = a.name, v = a.value;
        // id
        if (n === 'id') {
          annotation.id = v;
        }
        // events (on-*)
        else if (n.slice(0, 3) === 'on-') {
          node.removeAttribute(n);
          annotation.events.push({
            name: n.slice(3),
            value: v
          });
        }
        // bindings (other attributes)
        else {
          var b = this._parseNodeAttributeAnnotation(node, n, v);
          if (b) {
            annotation.bindings.push(b);
          }
        }
      }
    },

    // construct annotation data from a generic attribute, or undefined
    _parseNodeAttributeAnnotation: function(node, n, v) {
      var mode = '', escape = v.slice(0, 2), name = n;
      if (escape === '{{' || escape === '[[') {
        // Mode (one-way or two)
        mode = escape[0];
        v = v.slice(2, -2);
        // Negate
        var not = false;
        if (v[0] == '!') {
          v = v.substring(1);
          not = true;
        }
        // Attribute or property
        var kind = 'property';
        if (n[n.length-1] == '$') {
          name = n.slice(0, -1);
          kind = 'attribute';
        }
        // Custom notification event
        var notifyEvent, colon;
        if (mode == '{' && (colon = v.indexOf('::')) > 0) {
          notifyEvent = v.substring(colon + 2);
          v = v.substring(0, colon);
        }
        // Remove annotation
        node.removeAttribute(n);
        // Case hackery: attributes are lower-case, but bind targets 
        // (properties) are case sensitive. Gambit is to map dash-case to 
        // camel-case: `foo-bar` becomes `fooBar`.
        // Attribute bindings are excepted.
        if (kind === 'property') {
          name = Polymer.CaseMap.dashToCamelCase(name);
        }
        return {
          kind: kind,
          mode: mode,
          name: name,
          value: v,
          negate: not,
          event: notifyEvent
        };
      }
    },

    // instance-time

    _localSubTree: function(node, host) {
      return (node === host) ? node.childNodes :
         (node.lightChildren || node.childNodes);
    },

    findAnnotatedNode: function(root, annote) {
      // recursively ascend tree until we hit root
      var parent = annote.parent && 
        Polymer.Annotations.findAnnotatedNode(root, annote.parent);
      // unwind the stack, returning the indexed node at each level
      return !parent ? root : 
        Polymer.Annotations._localSubTree(parent, root)[annote.index];
    }

  };


