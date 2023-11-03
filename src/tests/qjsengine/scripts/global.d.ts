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
                open(): boolean;
                close(accepted: boolean): void;
            }

            interface FormLayout extends Element {
                addRow(label: string, element: Element): void;
                addElement(element: Element): void;
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

            interface ElementKeyMap {
                'button': Button;
                'dialog': Dialog;
                'form-layout': FormLayout;
                'label': Label;
                'line-edit': LineEdit
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