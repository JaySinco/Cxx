namespace cpp cyclone

struct SingleSelector {
    1: map<string, string> attrMap;
}

struct Selector {
    1: string strategyName;
    2: list<SingleSelector> singleSelectorList;
}

struct Point {
    1: i32 x;
    2: i32 y;
}

struct Rect {
    1: Point topLeft;
    2: i32 width;
    3: i32 height;
}

struct SelectResponse {
    1: i32 rtn;
    2: list<Rect> elements;
}

struct GrabResponse {
    1: i32 rtn;
    2: Rect rect;
    3: optional Selector selector;
}

struct MergeSimilarResponse {
    1: i32 rtn;
    2: Selector selector;
}

service CaptureStrategy {
    string Name();

    // 选择元素
    SelectResponse Select(
        1: Selector selector,
        2: list<Rect> priorElements);

    // 抓取元素
    GrabResponse Grab(
        1: Point mouse,
        2: Rect priorRect,
        3: string priorStrategyName,
        4: bool genSelectorList);

    // 生成相似元素
    MergeSimilarResponse MergeSimilar(1: list<Selector> selectorList);
}

service WindowsCapture extends CaptureStrategy {
}

service JavaCapture extends CaptureStrategy {
}

service ChromeCapture extends CaptureStrategy {
}

service UiStructedCapture extends CaptureStrategy {
    // 对指定区域结构化，并记录到对象内部
    i32 Detect(1: Rect rect);
}

