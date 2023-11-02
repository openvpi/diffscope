(()=> {
    $.registry.registerScript(class {
        static manifest() {
            return {
                id: "test",
                name: "Test",
            }
        }

        /**
         * @param {$.Project} project
         */
        constructor(project) {
            this.project = project;
            $.storage.setItem("a", "b");
        }

        prepare() {
            return this.project.window.question("Test?");
        }

        main() {
            this.project.window.alert($.storage.getItem("a"));
        }
    })

})()