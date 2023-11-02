(()=> {
    $.registry.registerScript(class {
        static manifest() {
            return {
                id: "test"
            }
        }

        constructor(project) {
            this.project = project;
        }

        prepare() {
            return this.project.window.question("Test?");
        }

        main() {
            this.project.window.alert("Test.");
        }
    })

})()