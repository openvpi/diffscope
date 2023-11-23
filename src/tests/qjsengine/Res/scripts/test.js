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
            let tests = [this.testDialog, this.testFile, this.testProgress, this.testConsole];
            let dlg = this.project.window.createDialog();
            let select = this.project.window.createElement('combo-box');
            dlg.content = select;
            tests.forEach(f => select.addOption(f.name));
            if (dlg.openDialog()) {
                this.testTarget = tests[select.currentIndex].bind(this);
                return true;
            } else {
                return false;
            }
        }

        main() {
            this.testTarget();
        }

        testDialog() {
            let dlg = this.project.window.createDialog();
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

            let select = this.project.window.createElement('combo-box');
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

            let boxLayout = this.project.window.createElement('box-layout');
            boxLayout.direction = $.Direction.LeftToRight;
            let btn1 = this.project.window.createElement('button');
            btn1.text = 'Test 1';
            let btn2 = this.project.window.createElement('button');
            btn2.text = 'Test 2';
            boxLayout.addElement(btn1);
            boxLayout.addElement(btn2);
            boxLayout.addStretch();
            dlgLayout.addElement(boxLayout);

            let gridLayout = this.project.window.createElement('grid-layout');
            gridLayout.addElement(dlgLayout, 0, 0);
            let grpBtn1 = this.project.window.createElement('radio');
            gridLayout.addElement(grpBtn1, 1, 1);
            let grpBtn2 = this.project.window.createElement('button');
            grpBtn2.checkable = true;
            gridLayout.addElement(grpBtn2, 1, 2);
            let grpBtn3 = this.project.window.createElement('check-box');
            gridLayout.addElement(grpBtn3, 1, 3);
            let btnGrp = this.project.window.createButtonGroup();
            btnGrp.addButton(grpBtn1);
            btnGrp.addButton(grpBtn2);
            btnGrp.addButton(grpBtn3);

            let stackedBtn = this.project.window.createElement('button');
            stackedBtn.text = 'Stacked';
            let stackedLayout = this.project.window.createElement('stacked-layout');
            stackedLayout.addElement(gridLayout);
            stackedLayout.addElement(stackedBtn);
            btn1.onClicked = () => {
                stackedLayout.currentIndex = 1;
            }
            stackedBtn.onClicked = () => {
                stackedLayout.currentIndex = 0;
            }

            let groupBox = this.project.window.createElement('group-box');
            groupBox.title = 'File Test';
            groupBox.checkable = true;
            let groupBoxLayout = this.project.window.createElement('form-layout');
            groupBox.content = groupBoxLayout;

            let fileSelector = this.project.window.createElement('file-selector');
            groupBoxLayout.addRow("File", fileSelector);
            btn2.onClicked = () => {
                fileSelector.allowsMultipleFiles = !fileSelector.allowsMultipleFiles;
            }

            let displayFileBtn = this.project.window.createElement('button');
            displayFileBtn.text = 'Test file';
            displayFileBtn.enabled = false;
            fileSelector.onFileChanged = () => {
                displayFileBtn.enabled = !!fileSelector.files.length;
            }
            displayFileBtn.onClicked = () => {
                let file = fileSelector.files[0];
                file.open($.OpenMode.ReadOnly);
                this.project.window.alert(file.readAllAsUtf8());
                file.close();
            }
            groupBoxLayout.addRow("Test", displayFileBtn);

            dlgLayout.addElement(groupBox);

            let renderedElement = this.project.window.renderElement({
                tag: 'group-box',
                attributes: {
                    title: 'Test',
                },
                children: [{
                    tag: 'form-layout',
                    children: [{
                        tag: 'button',
                        attributes: {
                            id: 'test-btn',
                        },
                        children: ["Test Button"],
                    }, {
                        tag: 'combo-box',
                        attributes: {
                            'form-layout-label': 'Test Select',
                            currentIndex: 2,
                        },
                        children: [{
                            tag: 'combo-box-option',
                            children: ["Option 1"],
                        }, {
                            tag: 'combo-box-option',
                            children: ["Option 2"],
                        }, {
                            tag: 'combo-box-option',
                            children: ["Option 3"],
                        }]
                    }]
                }],
            });
            this.project.window.getElementById('test-btn').onClicked = () => {
                this.project.window.alert("Test");
            }

            dlgLayout.addElement(renderedElement);

            let tabWidget = this.project.window.renderElement(`
                <tab-widget id="test-tab-widget">
                    <button id="tab-widget-btn-1" tab-widget-label="Tab 1">Button 1</button>
                    <button id="tab-widget-btn-2" tab-widget-label="Tab 2">Button 2</button>
                </tab-widget>
            `);
            dlgLayout.addElement(tabWidget);
            this.project.window.getElementById('tab-widget-btn-1').onClicked = () => {
                this.project.window.getElementById('test-tab-widget').currentIndex = 1;
            }

            dlg.content = stackedLayout;
            dlg.openDialog();
        }

        testProgress() {
            this.project.startProgress("Test progressing", 1000);
            for (let i = 0; i < 1000; i++) {
                this.project.updateProgress(i);
                let a = []
                for (let j = 0; j < 100; j++) {
                    for(let k = 0; k < 100; k++) {
                        a[Math.round(Math.abs(Math.cos(k)) * j)] = Math.log(j * k);
                    }
                }
            }
        }

        testFile() {
            let dialog = this.project.window.createDialog();
            let fileSelector = this.project.window.createElement('file-selector');
            dialog.content = fileSelector;
            fileSelector.onFileChanged = () => {
                if (!fileSelector.files.length)
                    return;
                let file = fileSelector.files[0];
                file.open($.OpenMode.WriteOnly);
                let data = new Uint8Array([52, 53, 54]);
                file.write(data);
            };
            dialog.openDialog();
        }

        testConsole() {
            console.debug(111);
            console.info("222\n222");
            console.log(333);
            console.warn(444);
            console.error(555);
            console.trace(666);
        }
    });

})();