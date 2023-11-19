((Class, manifest_) => {
    if (manifest_.isScriptSet) {
        $.registry.registerScript(class {
            static manifest() {
                return manifest_;
            }

            constructor(project) {
                this.clazz = new Class(project);
            }

            prepare(index) {
                return this.clazz.prepare(index);
            }

            main() {
                this.clazz.main();
            }
        });
    } else {
        $.registry.registerScript(class {
            static manifest() {
                return manifest_;
            }

            constructor(project) {
                this.clazz = new Class(project);
            }

            prepare() {
                return this.clazz.prepare();
            }

            main() {
                this.clazz.main();
            }
        });
    }
});
