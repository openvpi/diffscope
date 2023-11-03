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
            let dlg = this.project.window.createElement('dialog');
            let dlgLayout = this.project.window.createElement('form-layout');
            let btn = this.project.window.createElement('button');
            btn.text = '111';
            dlgLayout.addRow('aaa', btn);
            let label = this.project.window.createElement('label');
            label.text = '0';
            btn.onClicked = () => {
                label.text = String(parseInt(label.text) + 1);
                if (parseInt(label.text) > 16)
                    dlg.close(true);
            }
            dlgLayout.addElement(label);
            let lineEdit = this.project.window.createElement('line-edit');
            lineEdit.text = '111';
            lineEdit.onTextChanged = (text) => {
                btn.text = text;
            }
            dlgLayout.addElement(lineEdit);
            dlg.content = dlgLayout;
            dlg.open();
        }
    })

})()