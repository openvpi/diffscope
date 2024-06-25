declare module 'diffscope' {
    export interface FileGlobal {
        author: string;
        projectName: string;
        centShift: number;

        readonly authorChanged: Qt.Signal<[string]>;
        readonly projectNameChanged: Qt.Signal<[string]>;
        readonly centShiftChanged: Qt.Signal<[number]>;
    }
}