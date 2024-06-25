declare module 'diffscope' {
    export interface BusControl {
        gain: number;
        pan: number;
        mute: boolean;

        readonly gainChanged: Qt.Signal<[number]>;
        readonly panChanged: Qt.Signal<[number]>;
        readonly muteChanged: Qt.Signal<[boolean]>;
    }
}