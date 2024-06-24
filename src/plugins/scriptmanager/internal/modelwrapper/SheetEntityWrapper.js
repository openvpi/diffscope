(() => {
    const methods = {
        add(item) {
            this.insert(item);
        },
        delete(item) {
            return this.remove(item);
        },
        get(index) {
            return this.value(index);
        },
        indexOf(item) {
            return this.indexOf(item);
        },
        indices() {
            return this.ids();
        },
        items() {
            return this.ids().map(id => this.value(id));
        },
        [Symbol.iterator]() {
            const it = this.ids()[Symbol.iterator]();
            return {
                next: () => {
                    let res = it.next();
                    return {
                        value: res.done ? undefined : this.value(res.value),
                        done: res.done,
                    }
                }
            };
        },
    };
    const properties = {
        size() {
            return this.size();
        },
        inserted() {
            return this.inserted;
        },
        aboutToRemove() {
            return this.aboutToRemove;
        },
        removed() {
            return this.removed;
        }
    };
    return (sheetEntityObject) => {
        return new Proxy(sheetEntityObject, {
            get(target, prop) {
                if (prop in properties)
                    return properties[prop].bind(target)();
                if (prop in methods) {
                    return methods[prop].bind(target);
                }
            },
            set() {
                return false;
            },
            deleteProperty() {
                return false;
            },
            ownKeys(target) {
                return target.ids();
            },
            has(target, prop) {
                return Number(prop) in target.ids() || prop in properties || prop in methods;
            },
            getOwnPropertyDescriptor(target, prop) {
                if (Number(prop) in target.ids()) {
                    return {
                        value: target.value(index),
                        writable: true,
                        enumerable: true,
                        configurable: true
                    };
                }
            }
        });
    }
})()