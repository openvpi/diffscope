declare namespace $ {

    namespace Project {
        namespace Window {
            interface Widget {}

            interface Dialog extends Widget {
                content: Widget;
                open(): boolean;
                close(accepted: boolean): void;
            }

            interface WidgetKeyMap {
                'dialog': Dialog;
            }
        }
    }

    interface Project {
        readonly window: {
            alert(message: string, title?: string): void;
            warning(message: string, title?: string): void;
            critical(message: string, title?: string): void;
            question(message: string, title?: string): boolean;
            createWidget<K extends keyof Project.Window.WidgetKeyMap>(tag: K): Project.Window.WidgetKeyMap[K];
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