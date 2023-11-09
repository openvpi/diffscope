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
                    dlg.closeDialog(true);
            }
            dlgLayout.addElement(label);

            let lineEdit = this.project.window.createElement('line-edit');
            lineEdit.text = '111';
            lineEdit.onTextChanged = (text) => {
                btn.text = text;
            }
            dlgLayout.addElement(lineEdit);

            let select = this.project.window.createElement('select');
            select.addOption('a');
            select.addOption('b');
            select.addOption('c');
            select.currentIndex = 1;
            select.onCurrentIndexChanged = (index) => {
                lineEdit.text = select.optionAt(index);
            }
            dlgLayout.addElement(select);

            let spinBox = this.project.window.createElement('spin-box');
            dlgLayout.addElement(spinBox);

            let slider = this.project.window.createElement('slider');
            slider.hasTicks = true;
            dlgLayout.addElement(slider);

            spinBox.onValueChanged = (value) => {
                slider.value = value;
            }
            slider.onValueChanged = (value) => {
                spinBox.value = value;
            }

            dlg.content = dlgLayout;
            dlg.content = null;
            dlg.openDialog();
        }
    })

})()