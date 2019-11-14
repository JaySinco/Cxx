#include <Windows.h>
#include <iostream>
#include <vector>

struct Coordinate {
    int x;
    int y;
};

struct RectangularRegion {
    Coordinate topLeftCoord;
    int height;
    int width;
};

std::ostream &operator<<(std::ostream &out, const RectangularRegion &region) {
   out << "(x=" << region.topLeftCoord.x << ", y=" << region.topLeftCoord.y
        << ", w=" << region.width << ", h=" << region.height << ")";
    return out;
}

struct Color {
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a; // not used, placeholder!
};

std::ostream &operator<<(std::ostream &out, const Color &color) {
    out << "(r=" << int(color.r) << ", g=" << int(color.g) << ", b=" << int(color.b) << ")";
    return out;
}

class ImageMat {
public:
    ImageMat(Color *data, int width, int height): data_(data), height_(height), width_(width) {}
    ImageMat(const ImageMat&) = delete;
    ImageMat(ImageMat &&mat): data_(mat.data_), height_(mat.height_), width_(mat.width_) { mat.data_ = nullptr; }
    ~ImageMat() { delete [] data_; }
    ImageMat &operator=(const ImageMat&) = delete;
    ImageMat &operator=(ImageMat&&) = delete;
    Color at(int row, int col) {
        if (!(row >= 0 && row < height_ && col >= 0 && col < width_)) {
            std::cout << "wrong color index, row=" << row << ", col=" << col << std::endl;
            exit(-1);
        }
        return data_[row*width_+col];
    }
    int Width() { return width_; }
    int Height() { return height_; }
    static ImageMat fromHWND(HWND hwnd);
private:
    Color *data_;
    int width_;
    int height_;
};

ImageMat ImageMat::fromHWND(HWND hwnd) {
    HDC hdc = GetDC(hwnd);
    RectangularRegion region;
    if (hwnd == NULL) {
        region.topLeftCoord.x = 0;
        region.topLeftCoord.y = 0;
        region.width = GetDeviceCaps(hdc, HORZRES);
        region.height = GetDeviceCaps(hdc, VERTRES);
    } else {
        RECT rect;
        GetClientRect(hwnd, &rect);
        region.topLeftCoord.x = rect.left;
        region.topLeftCoord.y = rect.top;
        region.width = rect.right - rect.left;
        region.height = rect.bottom - rect.top;
    }
    std::cout << "RectangularRegion: " << region << std::endl;
    HDC hBitmapDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, region.width, region.height);
    SelectObject(hBitmapDC, hBitmap);
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = region.width;
    bi.biHeight = -region.height;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
    StretchBlt(hBitmapDC, 0, 0, region.width, region.height,
        hdc, region.topLeftCoord.x, region.topLeftCoord.y, region.width, region.height, SRCCOPY);
    auto data = new Color[region.width * region.height] { 0 };
    GetDIBits(hBitmapDC, hBitmap, 0, region.height, data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    DeleteObject(hBitmap);
    DeleteDC(hBitmapDC);
    ReleaseDC(NULL, hdc);
    return { data, region.width, region.height};
}

Color pixel2Color(int row, int col) {
    HDC hdc = GetDC(NULL);
    COLORREF color = GetPixel(hdc, col, row);
    Color c = { 0 };
    c.r = GetRValue(color);
    c.g = GetGValue(color);
    c.b = GetBValue(color);
    ReleaseDC(0, hdc);
    return c;
}

void drawPoints(const std::vector<Coordinate> &points, double msExpired) {
    HDC hdc = GetDC(NULL);
    for (const auto pt: points) {
        SetPixel(hdc, pt.x, pt.y, RGB(0, 192, 142));
    }
    Sleep(msExpired);
    RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE|RDW_ALLCHILDREN);
    ReleaseDC(NULL, hdc);
}

void selectSameColor() {
    //...
}

// ************** TEST & MAIN **************

void testImageMatFromHWND() {
    std::cout << "sizeOfColor=" << sizeof(Color) << std::endl;
    Sleep(2000);
    POINT mousePos;
    GetCursorPos(&mousePos);
    std::cout << "cursorPos=(" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
    std::cout << "pixel2Color" << pixel2Color(mousePos.y, mousePos.x) << std::endl;
    ImageMat img = ImageMat::fromHWND(NULL);
    std::cout << "hWnd2Image" << img.at(mousePos.y, mousePos.x) << std::endl;
}

void testDrawPoints() {
    std::vector<Coordinate> points;
    for (int x = 0; x < 100; ++x) {
        for (int y = 0; y < 200; ++y) {
            if (x > y)
                points.push_back({x, y});
        }
    }
    drawPoints(points, 2000);
}

int main() {

}