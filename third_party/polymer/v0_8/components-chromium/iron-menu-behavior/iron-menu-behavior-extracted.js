

  Polymer.IronMenuBehavior = Polymer.IronMultiSelectableBehavior.concat({

    properties: {

      /**
       * Returns the currently focused item.
       *
       * @attribute focusedItem
       * @type Object
       */
      focusedItem: {
        observer: '_focusedItemChanged',
        readOnly: true,
        type: Object
      },

      /**
       * The attribute to use on menu items to look up the item title. Typing the first
       * letter of an item when the menu is open focuses that item. If unset, `textContent`
       * will be used.
       *
       * @attribute attrForItemTitle
       * @type String
       */
      attrForItemTitle: {
        type: String
      }

    },

    observers: [
      '_selectedItemsChanged(selectedItems)',
      '_selectedItemChanged(selectedItem)'
    ],

    hostAttributes: {
      'role': 'menu',
      'tabindex': '0'
    },

    listeners: {
      'focus': '_onFocus',
      'keydown': '_onKeydown'
    },

    _focusedItemChanged: function(focusedItem, old) {
      old && old.setAttribute('tabindex', '-1');
      if (focusedItem) {
        focusedItem.setAttribute('tabindex', '0');
        focusedItem.focus();
      }
    },

    _selectedItemsChanged: function(selectedItems) {
      this._setFocusedItem(selectedItems[0]);
    },

    _selectedItemChanged: function(selectedItem) {
      this._setFocusedItem(selectedItem);
    },

    _onFocus: function(event) {
      // clear the cached focus item
      this._setFocusedItem(null);
      // focus the selected item when the menu receives focus, or the first item
      // if no item is selected
      var selectedItem = this.multi ? (this.selectedItems && this.selectedItems[0]) : this.selectedItem;
      if (selectedItem) {
        this._setFocusedItem(selectedItem);
      } else {
        this._setFocusedItem(this.items[0]);
      }
    },

    _onKeydown: function(event) {
      // FIXME want to define these somewhere, core-a11y-keys?
      var DOWN = 40;
      var UP = 38;
      var ESC = 27;
      var ENTER = 13;
      if (event.keyCode === DOWN) {
        // up and down arrows moves the focus
        this._focusNext();
      } else if (event.keyCode === UP) {
        this._focusPrevious();
      } else if (event.keyCode === ESC) {
        // esc blurs the control
        this.focusedItem.blur();
      } else if (event.keyCode === ENTER) {
        // enter activates the item unless it is disabled
        if (!this.focusedItem.hasAttribute('disabled')) {
          this._activateHandler(event);
        }
      } else {
        // all other keys focus the menu item starting with that character
        for (var i = 0, item; item = this.items[i]; i++) {
          var attr = this.attrForItemTitle || 'textContent';
          var title = item[attr] || item.getAttribute(attr);
          if (title && title.trim().charAt(0).toLowerCase() === String.fromCharCode(event.keyCode).toLowerCase()) {
            this._setFocusedItem(item);
            break;
          }
        }
      }
    },

    _focusPrevious: function() {
      var length = this.items.length;
      var index = (Number(this.indexOf(this.focusedItem)) - 1 + length) % length;
      this._setFocusedItem(this.items[index]);
    },

    _focusNext: function() {
      var index = (Number(this.indexOf(this.focusedItem)) + 1) % this.items.length;
      this._setFocusedItem(this.items[index]);
    }

  });

