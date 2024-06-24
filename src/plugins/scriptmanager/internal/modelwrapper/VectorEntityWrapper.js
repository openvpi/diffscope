(() => {
    const methods = {
        push(...args) {
            this.insert(this.size(), args);
        },
        pop() {
            if (this.size() == 0)
                return undefined;
            const length = this.size()
            const ret = this.at(length - 1);
            this.remove(length - 1, 1);
            return ret;
        },
        unshift(...args) {
            this.insert(0, args);
        },
        shift() {
            if (this.size() == 0)
                return undefined;
            const ret = this.at(0);
            this.remove(0, 1);
            return ret;
        },
        splice(start, deleteCount, ...args) {
            const size = this.size();
            start = start < 0 ? Math.max(size + start, 0) : Math.min(start, size);
            deleteCount = deleteCount === undefined ? size - start : Math.min(Math.max(deleteCount, 0), size - start);
        
            const removedItems = [];
            for (let i = 0; i < deleteCount; i++) {
                removedItems.push(this.at(start));
                this.remove(start, 1);
            }
        
            if (args.length > 0) {
                this.insert(start, items);
            }
        
            return removedItems;
        },
        moveWithin(target, start, end) {
            const size = this.size();
            target = target < 0 ? Math.max(size + target, 0) : Math.min(target, size);
            start = start < 0 ? Math.max(size + start, 0) : Math.min(start, size);
            end = end === undefined ? size : (end < 0 ? Math.max(size + end, 0) : Math.min(end, size));

            if (target >= size || start >= end) {
                return;
            }

            this.move(start, end - start, target);
        },
        [Symbol.iterator]() {
            let index = 0;
            const size = this.size();
            return {
                next: () => {
                    if (index < size) {
                        return { value: this.at(index++), done: false };
                    } else {
                        return { value: undefined, done: true };
                    }
                }
            };
        },
    };
    const properties = {
        length() {
            return this.size();
        },
        inserted() {
            return this.inserted;
        },
        aboutToMove() {
            return this.aboutToMove;
        },
        moved() {
            return this.moved;
        },
        aboutToRemove() {
            return this.aboutToRemove;
        },
        removed() {
            return this.removed;
        }
    };
    return (vectorEntityObject) => {
        return new Proxy(vectorEntityObject, {
            get(target, prop) {
                if (!isNaN(prop))
                    return target.at(Number(prop));
                if (prop in properties)
                    return properties[prop].bind(target)();
                if (prop in methods) {
                    return methods[prop].bind(target);
                }
            },
            set(target, prop, value) {
                if (!isNaN(prop)) {
                    const index = Number(prop);
                    if (index < 0 || index >= target.size()) {
                        return false;
                    } else {
                        target.remove(index, 1);
                        target.insert(index, [value]);
                    }
                    return true;
                }
                return false;
            },
            deleteProperty(target, prop) {
                if (!isNaN(prop)) {
                    const index = Number(prop);
                    if (index < target.size()) {
                        target.remove(index, 1);
                    }
                    return true;
                }
                return false;
            },
            ownKeys(target) {
                const keys = [];
                for (let i = 0; i < target.size(); i++) {
                    keys.push(i.toString());
                }
                return keys;
            },
            has(target, prop) {
                if (!isNaN(prop)) {
                    const index = Number(prop);
                    return index >= 0 && index < target.size();
                }
                return prop in properties || prop in methods;
            },
            getOwnPropertyDescriptor(target, prop) {
                if (!isNaN(prop)) {
                    const index = Number(prop);
                    if (index >= 0 && index < target.size()) {
                        return {
                            value: target.at(index),
                            writable: true,
                            enumerable: true,
                            configurable: true
                        };
                    }
                }
            }
        });
    }
})()