declare namespace $ {

    namespace Project {
        namespace Window {
            interface Element {}

            interface WidgetElement extends Element{
                enabled: boolean;
                visible: boolean;
                toolTip: string;
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

            interface Dialog extends Element {
                content: Element;
                openDialog(): boolean;
                closeDialog(accepted: boolean): void;
            }

            interface FormLayout extends Element {
                addRow(label: string, element: Element): void;
                addElement(element: Element): void;
                horizontalSpacing: number;
                verticalSpacing: number;
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
                'button': Button;
                'dialog': Dialog;
                'double-spin-box': DoubleSpinBox;
                'form-layout': FormLayout;
                'label': Label;
                'line-edit': LineEdit;
                'select': Select;
                'slider': Slider;
                'spin-box': SpinBox;
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