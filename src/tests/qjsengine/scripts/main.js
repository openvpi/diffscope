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

            let tabWidget = this.project.window.renderElement({
                tag: 'tab-widget',
                children: [{
                    tag: 'button',
                    attributes: {
                        'tab-widget-label': 'Tab 1',
                    },
                    children: ['Button 1'],
                }, {
                    tag: 'button',
                    attributes: {
                        'tab-widget-label': 'Tab 2',
                    },
                    children: ['Button 2'],
                }],
            });
            dlgLayout.addElement(tabWidget);

            dlg.content = stackedLayout;
            dlg.openDialog();
        }
    })

})()