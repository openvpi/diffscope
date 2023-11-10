declare namespace $ {

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

            interface Dialog extends Element {
                content: Element|null;
                openDialog(): boolean;
                closeDialog(accepted: boolean): void;
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

            interface Select extends WidgetElement {
                currentIndex: number;
                addOption(text: string): void;
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

            interface ElementKeyMap {
                'box-layout': BoxLayout;
                'button': Button;
                'check-box': CheckBox;
                'dialog': Dialog;
                'double-spin-box': DoubleSpinBox;
                'form-layout': FormLayout;
                'grid-layout': GridLayout;
                'label': Label;
                'line-edit': LineEdit;
                'radio': Radio;
                'select': Select;
                'slider': Slider;
                'spin-box': SpinBox;
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
        }
    }

    interface Project {
        readonly window: {
            alert(message: string, title?: string): void;
            warning(message: string, title?: string): void;
            critical(message: string, title?: string): void;
            question(message: string, title?: string): boolean;
            createElement<K extends keyof Project.Window.ElementKeyMap>(tag: K): Project.Window.ElementKeyMap[K];
            createButtonGroup(): Project.Window.ButtonGroup;
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