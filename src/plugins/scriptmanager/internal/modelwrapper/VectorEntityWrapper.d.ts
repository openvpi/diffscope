declare module 'diffscope' {
    export interface VectorEntityWrapper<T> extends Iterable<T> {
        [K: number]: T;
        length: number;
    
        push(...items: T[]): number;
        pop(): T | undefined;
        unshift(...items: T[]): number;
        shift(): T | undefined;
        splice(start: number, deleteCount: number, ...items: T[]): T[];
        moveWithin(target: number, start: number, end?: number): void;

        readonly inserted: Qt.Signal<[number, T[]]>;
        readonly aboutToMove: Qt.Signal<[number, number, number]>;
        readonly moved: Qt.Signal<[number, number, number]>;
        readonly aboutToRemove: Qt.Signal<[number, T[]]>;
        readonly removed: Qt.Signal<[number, number]>;
    }
}

