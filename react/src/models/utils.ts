import { CSSProperties } from "react";

export interface CursorPosition {
    x: number;
    y: number;
}

export interface CSSPosition {
    top: string;
    left: string;
    visibility: CSSProperties['visibility'];
}
