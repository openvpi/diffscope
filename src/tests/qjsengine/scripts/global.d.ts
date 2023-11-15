declare namespace $ {

    import ElementDescription = $.Project.Window.ElementDescription;
    import RenderedIdMap = $.Project.Window.RenderedIdMap;

    enum Alignment {
        None          = 0x0000,
        AlignLeft     = 0x0001,
        AlignRight    = 0x0002,
        AlignHCenter  = 0x0004,
        AlignJustify  = 0x0008,
        AlignTop      = 0x0020,
        AlignBottom   = 0x0040,
        AlignVCenter  = 0x0080,
        AlignBaseline = 0x0100,
        AlignAbsolute = 0x0010,
    }

    enum CheckState {
        Unchecked,
        PartiallyChecked,
        Checked,
    }

    enum Corner {
        TopLeftCorner,
        TopRightCorner,
        BottomLeftCorner,
        BottomRightCorner,
    }

    enum Direction {
        LeftToRight,
        RightToLeft,
        TopToBottom,
        BottomToTop,
    }

    enum OpenMode {
        NotOpen = 0x0000,
        ReadOnly = 0x0001,
        WriteOnly = 0x0002,
        ReadWrite = 0x0003,
        Append = 0x0004,
        Truncate = 0x0008,
        Text = 0x0010,
    }

    enum TabPosition {
        North,
        South,
        West,
        East,
    }

    interface File {
        readonly name: string;
        readonly lastModifiedDate: Date|null;
        size: number;
        pos: number;
        openMode: OpenMode;

        open(openMode: OpenMode): boolean;
        close(): void;
        read(readSize: number): Uint8Array;
        readAll(): Uint8Array;
        readAllAsUtf8(): string;
        write(data: string|number[]|Uint8Array): number;
        flush(): void;
    }

    namespace Project {
        namespace Window {
            interface Element {}

            interface WidgetElement extends Element{
                enabled: boolean;
                visible: boolean;
                toolTip: string;
            }

            interface BoxLayout {
                direction: Direction;
                addElement(element: Element, stretch = 0, alignment = Alignment.None): void,
                addSpacing(size: number): void;
                addStretch(stretch = 0): void;
                addStrut(size: number): void;

                insertElement(index: number, element: Element, stretch: number = 0, alignment = Alignment.None): void;
                insertSpacing(index: number, size: number): void;
                insertStretch(index: number, stretch = 0): void;

                spacing: number;
                readonly count: number;
            }

            interface Button extends WidgetElement {
                autoExclusive: boolean;
                checkable: boolean;
                checked: boolean;
                text: string;
                onClicked: ((isChecked: boolean) => void)|null;
                onPressed: (() => void)|null;
                onReleased: (() => void)|null;
            }

            interface CheckBox extends Button {
                tristate: boolean;
                checkState: CheckState;
                onStateChanged: ((state: CheckState) => void)|null;
            }

            interface Dialog {
                content: Element|null;
                openDialog(): boolean;
                closeDialog(accepted: boolean): void;
            }

            interface FileSelector extends WidgetElement {
                isOpenFile: boolean;
                allowsMultipleFiles: boolean;
                filter: string;
                title: string;
                selectedFilter: string;
                files: $.File[];
                onFileChanged: (() => void)|null;
            }

            interface FormLayout extends Element {
                addRow(label: string, element: Element): void;
                addElement(element: Element): void;

                insertRow(row: number, label: string, element: Element): void;
                insertElement(row: number, element: Element): void;

                horizontalSpacing: number;
                verticalSpacing: number;
                readonly rowCount: number;
            }

            interface GridLayout extends Element {
                addElement(element: Element, row: number, column: number, alignment = Alignment.None): void;
                addElement(element: Element, row: number, column: number, rowSpan: number, columnSpan: number, alignment = Alignment.None): void;

                rowMinimumHeight(row: number): number;
                columnMinimumWidth(column: number): number;
                setRowMinimumHeight(row: number, minSize: number): number;
                setColumnMinimumWidth(column: number, minSize: number): number;

                rowStretch(row: number): number;
                columnStretch(column: number): number;
                setRowStretch(row: number, stretch: number): number;
                setColumnStretch(column: number, stretch: number): number;

                horizontalSpacing: number;
                verticalSpacing: number;
                readonly rowCount: number;
                readonly columnCount: number;
                readonly count: number;
                originCorner: Corner;

            }

            interface GroupBox extends WidgetElement {
                alignment: Alignment;
                checkable: boolean;
                checked: boolean;
                title: string;
                content: Element;
                onClicked: ((checked: boolean) => void)|null;
                onToggled: ((on: boolean) => void)|null;
            }

            interface Label extends WidgetElement {
                text: string;
            }

            interface LineEdit extends WidgetElement {
                readonly acceptableInput: boolean;
                cursorPosition: number;
                hasSelectedText: boolean;
                inputMask: string;
                maxLength: number;
                placeholderText: string;
                readOnly: boolean;
                readonly selectedText: string;
                text: string;
                clear(): void;
                copy(): void;
                selectAll(): void;
                onCursorPositionChanged: ((oldPos: number, newPos: number) => void)|null;
                onEditingFinished: (() => void)|null;
                onInputRejected: (() => void)|null;
                onReturnPressed: (() => void)|null;
                onSelectionChanged: (() => void)|null;
                onTextChanged: ((text: string) => void)|null;
                onTextEdited: ((text: string) => void)|null;
            }

            interface Radio extends Button {

            }

            interface ComboBox extends WidgetElement {
                currentIndex: number;
                addOption(text: string): void;
                insertOption(index: number, text: string): void;
                removeOption(index: number): void;
                clear(): void;
                optionAt(index: number): string;
                onCurrentIndexChanged: ((index: number) => void)|null;
            }

            interface Slider extends WidgetElement {
                maximum: number;
                minimum: number;
                pageStep: number;
                singleStep: number;
                value: number;
                tickInterval: number;
                hasTicks: boolean;
                onValueChanged: ((value: number) => void)|null;
            }

            interface SpinBox extends WidgetElement {
                readOnly: boolean;
                maximum: number;
                minimum: number;
                prefix: string;
                singleStep: number;
                suffix: string;
                value: number;
                onValueChanged: ((value: number) => void)|null;
            }

            interface DoubleSpinBox extends SpinBox {
                decimals: number;
            }

            interface StackedLayout extends Element {
                addElement(element: Element): void;
                insertElement(index: number, element: Element);
                readonly count: number;
                currentIndex: number;
            }

            interface TabWidget extends WidgetElement {
                readonly count: number;
                currentIndex: number;
                tabPosition: TabPosition;
                usesScrollButtons: boolean;
                addTab(element: Element, label: string): number;
                insertTab(index: number, element: Element, label: string): number;
                setTabEnabled(index: number, enabled: boolean): void;
                isTabEnabled(index: number): boolean;
                setTabText(index: number, label: string): void;
                tabText(index: number): string;
                setTabToolTip(index: number, tip: string): void;
                tabToolTip(index: number): string;
                setTabVisible(index: number, visible: boolean): void;
                isTabVisible(index: number): boolean;

                onCurrentChanged: ((index: number) => void)|null;
                onTabBarClicked: ((index: number) => void)|null;
                onTabBarDoubleClicked: ((index: number) => void)|null;
            }

            interface ElementKeyMap {
                'box-layout': BoxLayout;
                'button': Button;
                'check-box': CheckBox;
                'double-spin-box': DoubleSpinBox;
                'file-selector': FileSelector;
                'form-layout': FormLayout;
                'grid-layout': GridLayout;
                'group-box': GroupBox;
                'label': Label;
                'line-edit': LineEdit;
                'radio': Radio;
                'combo-box': ComboBox;
                'slider': Slider;
                'spin-box': SpinBox;
                'stacked-layout': StackedLayout;
                'tab-widget': TabWidget;
            }

            interface ButtonGroup {
                addButton(button: Button, id = -1): void;
                checkedId(): number;
                id(button: button): number;
                removeButton(button: Button): void;
                setId(button: Button, id: number): void;
                onIdClicked: ((id: number) => void)|null;
                onIdPressed: ((id: number) => void)|null;
                onIdReleased: ((id: number) => void)|null;
                onIdToggled: ((id: number, checked: boolean) => void)|null;

            }

            interface ElementDescription {
                tag: string;
                attributes?: {
                    [attribute: string]: any;
                }
                children?: (ElementDescription|string)[];
            }
        }
    }

    interface Project {
        readonly window: {
            alert(message: string, title?: string): void;
            warning(message: string, title?: string): void;
            critical(message: string, title?: string): void;
            question(message: string, title?: string): boolean;
            createDialog(): Project.Window.Dialog;
            createElement<K extends keyof Project.Window.ElementKeyMap>(tag: K): Project.Window.ElementKeyMap[K];
            createButtonGroup(): Project.Window.ButtonGroup;
            renderElement(description: ElementDescription|string);
            getElementById(id: string): Project.Window.Element|null;
        }
    }

    interface ScriptManifestBase {
        id: string;
        name: string;
    }

    interface ScriptManifest extends ScriptManifestBase {
        isScriptSet?: false;
    }

    interface ScriptSetManifest extends ScriptManifestBase {
        isScriptSet: true;
    }

    interface ScriptConstructor {
        manifest(): ScriptManifest;
        new(project: Project): {
            prepare(): boolean;
            main(): void;
        }
    }

    interface ScriptSetConstructor {
        manifest(): ScriptSetManifest;
        new(project: Project): {
            prepare(index: number): boolean;
            main(): void;
        }
    }

    type ScriptLike = ScriptConstructor|ScriptSetConstructor;

    const registry: {
        registerScript(script: ScriptLike): void;
    }

    const storage: {
        setItem(key: string, value: string): void;
        getItem(key: string): string|null;
        removeItem(key: string): void;
    }
}