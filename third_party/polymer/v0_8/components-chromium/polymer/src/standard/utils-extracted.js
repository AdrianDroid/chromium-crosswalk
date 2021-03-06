

  Polymer.Base._addFeature({

    $$: function(slctr) {
      return Polymer.dom(this.root).querySelector(slctr);
    },

    toggleClass: function(name, bool, node) {
      node = node || this;
      if (arguments.length == 1) {
        bool = !node.classList.contains(name);
      }
      if (bool) {
        node.classList.add(name);
      } else {
        node.classList.remove(name);
      }
    },

    toggleAttribute: function(name, bool, node) {
      (node || this)[bool ? 'setAttribute' : 'removeAttribute'](name, '');
    },

    classFollows: function(className, neo, old) {
      if (old) {
        old.classList.remove(className);
      }
      if (neo) {
        neo.classList.add(className);
      }
    },

    attributeFollows: function(name, neo, old) {
      if (old) {
        old.removeAttribute(name);
      }
      if (neo) {
        neo.setAttribute(name, '');
      }
    },

    getContentChildNodes: function(slctr) {
      return Polymer.dom(Polymer.dom(this.root).querySelector(
          slctr || 'content')).getDistributedNodes();
    },

    getContentChildren: function(slctr) {
      return this.getContentChildNodes(slctr).filter(function(n) {
        return (n.nodeType === Node.ELEMENT_NODE);
      });
    },

    fire: function(type, detail, options) {
      options = options || Polymer.nob;
      var node = options.node || this;
      var detail = (detail === null || detail === undefined) ? Polymer.nob : detail;
      var bubbles = options.bubbles === undefined ? true : options.bubbles;
      var event = new CustomEvent(type, {
        bubbles: Boolean(bubbles),
        cancelable: Boolean(options.cancelable),
        detail: detail
      });
      node.dispatchEvent(event);
      return event;
    },

    async: function(method, waitTime) {
      return Polymer.Async.run(method.bind(this), waitTime);
    },

    cancelAsync: function(handle) {
      Polymer.Async.cancel(handle);
    },

    arrayDelete: function(array, item) {
      var index = array.indexOf(item);
      if (index >= 0) {
        return array.splice(index, 1);
      }
    },

    transform: function(node, transform) {
      node.style.webkitTransform = transform;
      node.style.transform = transform;
    },

    translate3d: function(node, x, y, z) {
      this.transform(node, 'translate3d(' + x + ',' + y + ',' + z + ')');
    },

    importHref: function(href, onload, onerror) {
      var l = document.createElement('link');
      l.rel = 'import';
      l.href = href;
      if (onload) {
        l.onload = onload.bind(this);
      }
      if (onerror) {
        l.onerror = onerror.bind(this);
      }
      document.head.appendChild(l);
      return l;
    },

    create: function(tag, props) {
      var elt = document.createElement(tag);
      if (props) {
        for (var n in props) {
          elt[n] = props[n];
        }
      }
      return elt;
    },

    simpleMixin: function(a, b) {
      for (var i in b) {
        a[i] = b[i];
      }
    }

  });

