#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include "./gen-cpp/CaptureStrategy.h"
#include "./gen-cpp/WindowsCapture.h"
#include "./gen-cpp/JavaCapture.h"
#include "./gen-cpp/ChromeCapture.h"
#include "./gen-cpp/UiStructedCapture.h"

#define CYCLONE_CAPTURE_SUCC 0
#define CYCLONE_CAPTURE_UNSATISFIED_CONDITION 1

using namespace cyclone;

template <class T>
void str2num(T &value, const std::string &s) {
  std::istringstream ss(s);
  ss >> value;
}

std::map<std::string, CaptureStrategyIf*> plugin = {
    // 策略需要满足一定的前提条件才返回成功，否则报错
    //  - 抓取到的区域比之前的更精确
    //  - UISTRUCTED   (优先级：0)
    //    * 内部维护一个已经结构化的区域(由Detect方法生成)，鼠标位置在这些区域中
    //  - WINDOWS      (优先级：1)
    //    * 当前是第一层
    //  - JAVA         (优先级：2)
    //    * 上一层是WINDOWS抓取，当前鼠标所在的窗口是java窗口
    //  - CHROME       (优先级：2)
    //    * 上一层是WINDOWS抓取，当前鼠标所在的窗口是chrome窗口
    { "JAVA",       new JavaCaptureIf },
    { "CHROME",     new ChromeCaptureIf },
    { "WINDOWS",    new WindowsCaptureIf },
    { "UISTRUCTED", new UiStructedCaptureIf },
};

enum class Rect2PointType {
    NONE,
    FOCUS,
    ANCHOR,
};

void Grab(
        // 输入
        Point mouse,
        Rect2PointType r2pType,
        bool genSelectorList,
        // 输出
        Rect &rect,
        std::vector<Selector> &selectorList)
{
    bool succ;
    Rect priorRect;
    std::string priorStrategyName;
    do {
        succ = false;
        for (const auto &strategy: plugin) {
            GrabResponse grabResp;
            strategy.second->Grab(grabResp, mouse, priorRect, priorStrategyName, genSelectorList);
            if (grabResp.rtn == CYCLONE_CAPTURE_SUCC) {
                succ = true;
                priorRect = grabResp.rect;
                priorStrategyName = strategy.first;
                if (genSelectorList)
                    selectorList.push_back(grabResp.selector);
                break;
            }
        }
    } while (succ);
    if (selectorList.size() > 0 && r2pType != Rect2PointType::NONE) {
        Selector last;
        if (r2pType == Rect2PointType::ANCHOR) {
            last.strategyName = "ANCHOR";
            SingleSelector singleSelector;
            singleSelector.attrMap["dx"] = mouse.x - priorRect.topLeft.x;
            singleSelector.attrMap["dy"] = mouse.y - priorRect.topLeft.y;
            last.singleSelectorList.push_back(singleSelector);
        }
        else if (r2pType == Rect2PointType::FOCUS) {
            last.strategyName = "FOCUS";
        }
        selectorList.push_back(last);
    }
}

void Select(
        // 输入
        const std::vector<Selector> &selectorList,
        // 输出
        std::vector<Rect> &elements)
{
    if (selectorList.size() < 0)
        return;
    std::vector<Rect> priorElements;
    for (const auto &selector: selectorList) {
        if (selector.strategyName == "ANCHOR") {
            int dx, dy;
            str2num(dx, selector.singleSelectorList[0].attrMap.at("dx"));
            str2num(dy, selector.singleSelectorList[0].attrMap.at("dy"));
            for (const auto &rect: priorElements) {
                Rect rect;
                rect.topLeft.x = rect.topLeft.x + dx;
                rect.topLeft.y = rect.topLeft.y + dy;
                rect.width = 0;
                rect.height = 0;
                elements.push_back(rect);
            }
            return;
        }
        if (selector.strategyName == "FOCUS") {
            for (const auto &rect: priorElements) {
                Rect rect;
                rect.topLeft.x = rect.topLeft.x + rect.width / 2;
                rect.topLeft.y = rect.topLeft.y + rect.height / 2;
                rect.width = 0;
                rect.height = 0;
                elements.push_back(rect);
            }
            return;
        }
        SelectResponse selectResp;
        plugin[selector.strategyName]->Select(selectResp, selector, priorElements);
        priorElements = selectResp.elements;
        if (priorElements.size() <= 0)
            break;
    }
    elements = priorElements;
}

