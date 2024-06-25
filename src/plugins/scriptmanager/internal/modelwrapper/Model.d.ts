declare module 'diffscope' {
    export interface Model {
        readonly global: FileGlobal;
        readonly master: Master;
        readonly timeline: any;
        readonly tracks: any;
    }
}