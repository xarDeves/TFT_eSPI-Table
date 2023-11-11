//BUG: rounding errors if incorrect values are passed to setRowHeight and setColumnWidth, relative to table's size

#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
class TableStack {
public:
    TableStack(TFT_eSPI* tft, uint16_t outlineColor, uint16_t fillColor);

    void generate();
    void draw() const;

    void eraseCell(uint8_t row, uint8_t column) const;
    void drawCellText(uint8_t row, uint8_t column, String text, uint16_t clr) const;
    void drawCellOutline(uint8_t row, uint8_t column) const;

    void setCellFillColor(uint8_t row, uint8_t column, uint16_t color);
    void setCellOutlineColor(uint8_t row, uint8_t column, uint16_t color);

    void setRowHeight(uint8_t row, uint16_t height);
    void setColumnWidth(uint8_t column, uint16_t width);

    void setPaddingTop(uint16_t top);
    void setPaddingBottom(uint16_t bottom);
    void setPaddingLeft(uint16_t left);
    void setPaddingRight(uint16_t right);

    uint16_t getX() const;
    uint16_t getY() const;
    uint16_t getWidth() const;
    uint16_t getHeight() const;
    uint8_t getRows() const;
    uint8_t getColumns() const;

    ~TableStack();

private:
    TFT_eSPI* tft;

    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;

    uint8_t rows;
    uint8_t columns;

    uint16_t paddingTop, paddingBottom, paddingLeft, paddingRight;

    uint16_t columnWidths[MaxColumns];
    uint16_t rowHeights[MaxRows];
    uint16_t cellCenters[MaxRows][MaxColumns][2];
    uint16_t cellXCoordinates[MaxRows][MaxColumns];
    uint16_t cellYCoordinates[MaxRows][MaxColumns];

    uint16_t outlineColor, fillColor;
    uint16_t cellFillColors[MaxRows][MaxColumns];
    uint16_t cellOutlineColors[MaxRows][MaxColumns];

    void calculateColumnWidths();
    void calculateRowHeights();
    void calculateCellCenters();
    void calculateCellCoordinates();
};

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::TableStack(TFT_eSPI* tft, uint16_t outlineColor, uint16_t fillColor) :
    tft(tft), x(X), y(Y), width(MaxWidth), height(MaxHeight),
    rows(MaxRows), columns(MaxColumns),
    outlineColor(outlineColor), fillColor(fillColor),
    paddingTop(0), paddingBottom(0), paddingLeft(0), paddingRight(0),
    columnWidths{0}, rowHeights{0},
    cellCenters{{0}},
    cellXCoordinates{{0}},
    cellYCoordinates{{0}} {

    for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < columns; j++) {
            cellFillColors[i][j] = fillColor;
            cellOutlineColors[i][j] = outlineColor;
        }
    }
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::generate() {
    calculateColumnWidths();
    calculateRowHeights();
    calculateCellCoordinates();
    calculateCellCenters();
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::calculateColumnWidths() {
    auto remainingWidth = MaxWidth - (paddingLeft + paddingRight);

    auto specifiedColumns = 0;
    for (auto i = 0; i < columns; i++) {
        if (columnWidths[i] != 0) {
            specifiedColumns++;
            remainingWidth -= columnWidths[i];
        }
    }

    float remainingColumns = columns - specifiedColumns;
    auto equalWidth = round(remainingWidth / remainingColumns);

    auto currentOffset = paddingLeft;
    for (auto i = 0; i < columns; i++) {
        if (columnWidths[i] == 0)
            columnWidths[i] = equalWidth;
        currentOffset += columnWidths[i];
    }
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::calculateRowHeights() {
    auto remainingHeight = MaxHeight - (paddingTop + paddingBottom);

    auto specifiedRows = 0;
    for (auto i = 0; i < rows; i++) {
        if (rowHeights[i] != 0) {
            specifiedRows++;
            remainingHeight -= rowHeights[i];
        }
    }

    auto remainingRows = rows - specifiedRows;
    float equalHeight = round(remainingHeight / remainingRows);

    auto currentOffset = paddingTop;
    for (auto i = 0; i < rows; i++) {
        if (rowHeights[i] == 0)
            rowHeights[i] = equalHeight;
        currentOffset += rowHeights[i];
    }
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::calculateCellCoordinates() {
    auto currentX = X + paddingLeft;
    auto currentY = Y + paddingTop;

    for (auto row = 0; row < rows; row++) {
        currentX = paddingLeft;
        for (auto column = 0; column < columns; column++) {
            cellXCoordinates[row][column] = currentX;
            currentX += columnWidths[column];
        }

        for (auto i = 0; i < columns; i++) {
            cellYCoordinates[row][i] = currentY;
        }

        currentY += rowHeights[row];
    }
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::calculateCellCenters() {
    for (auto row = 0; row < rows; row++) {
        auto trueY = Y + paddingTop;
        for (auto i = 0; i < row; i++) {
            trueY += rowHeights[i];
        }

        for (auto column = 0; column < columns; column++) {
            auto x = cellXCoordinates[row][column] + columnWidths[column] / 2;
            cellCenters[row][column][0] = x;
            cellCenters[row][column][1] = trueY + rowHeights[row] / 2;
        }
    }
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::draw() const {
    for (auto row = 0; row < rows; row++) {
        for (auto column = 0; column < columns; column++) {
            auto x = cellXCoordinates[row][column];
            auto y = cellYCoordinates[row][column];
            auto width = columnWidths[column];
            auto height = rowHeights[row];
            auto fillColor = cellFillColors[row][column];
            auto outlineColor = cellOutlineColors[row][column];

            tft->fillRect(x, y, width, height, fillColor);
            tft->drawRect(x, y, width, height, outlineColor);
        }
    }
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::drawCellText(uint8_t row, uint8_t column, String text, uint16_t clr) const {
    tft->setTextColor(clr);

    auto x = cellCenters[row][column][0];
    auto y = cellCenters[row][column][1];

    tft->drawString(text, x, y);

    tft->setTextColor(ILI9486_WHITE);
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::drawCellOutline(uint8_t row, uint8_t column) const {
    auto x = cellXCoordinates[row][column];
    auto y = cellYCoordinates[row][column];
    auto width = columnWidths[column];
    auto height = rowHeights[row];
    auto outlineColor = cellOutlineColors[row][column];

    tft->drawRect(x, y, width, height, outlineColor);
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::eraseCell(uint8_t row, uint8_t column) const {
    auto x = cellXCoordinates[row][column];
    auto y = cellYCoordinates[row][column];
    auto width = columnWidths[column];
    auto height = rowHeights[row];
    auto fillColor = cellFillColors[row][column];

    tft->fillRect(x + 1, y + 1, width - 2, height - 2, fillColor);
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::setCellFillColor(uint8_t row, uint8_t column, uint16_t color) {
    cellFillColors[row][column] = color;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::setCellOutlineColor(uint8_t row, uint8_t column, uint16_t color) {
    cellOutlineColors[row][column] = color;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::setRowHeight(uint8_t row, uint16_t height) {
    rowHeights[row] = height;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::setColumnWidth(uint8_t column, uint16_t width) {
    columnWidths[column] = width;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::setPaddingTop(uint16_t top) {
    paddingTop = top;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::setPaddingBottom(uint16_t bottom) {
    paddingBottom = bottom;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::setPaddingLeft(uint16_t left) {
    paddingLeft = left;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
void TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::setPaddingRight(uint16_t right) {
    paddingRight = right;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
uint16_t TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::getX() const {
    return X + paddingLeft;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
uint16_t TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::getY() const {
    return Y + paddingTop;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
uint8_t TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::getRows() const {
    return rows;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
uint8_t TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::getColumns() const {
    return columns;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
uint16_t TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::getWidth() const {
    uint16_t width = 0;
    for (uint8_t column = 0; column < columns; column++) {
        width += columnWidths[column];
    }
    return width;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
uint16_t TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::getHeight() const {
    uint16_t height = 0;
    for (uint8_t row = 0; row < rows; row++) {
        height += rowHeights[row];
    }
    return height;
}

template <uint16_t X, uint16_t Y, uint16_t MaxWidth, uint16_t MaxHeight, uint8_t MaxRows, uint8_t MaxColumns>
TableStack<X, Y, MaxWidth, MaxHeight, MaxRows, MaxColumns>::~TableStack() {}