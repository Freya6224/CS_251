#include "include/shape.h"

#include <sstream>
#include <string>

using namespace std;

Shape::Shape() {
  this->x = 0;
  this->y = 0;
}

Shape::Shape(int x, int y) {
  this->x = x;
  this->y = y;
}

Shape::~Shape() {
}

Shape* Shape::copy() {
  return new Shape(*this);
}

int Shape::getX() const {
  return x;
}

int Shape::getY() const {
  return y;
}

void Shape::setX(int xVal) {
  this->x = xVal;
}

void Shape::setY(int yVal) {
  this->y = yVal;
}

string Shape::as_string() const {
  ostringstream strRep;
  strRep << "It's a Shape at x: " << x << ", y: " << y;
  return strRep.str();
}

Rect::Rect() : Shape() {
  this->width = 0;
  this->height = 0;
}

Rect::Rect(int w, int h) : Shape() {
  this->width = w;
  this->height = h;
}

Rect::Rect(int x, int y, int w, int h) : Shape(x, y) {
  this->width = w;
  this->height = h;
}

Rect::~Rect() {
}
Rect* Rect::copy() {
  return new Rect(*this);
}

int Rect::getWidth() const {
  return width;
}

int Rect::getHeight() const {
  return height;
}

void Rect::setWidth(int w) {
  this->width = w;
}
void Rect::setHeight(int h) {
  this->height = h;
}

string Rect::as_string() const {
  ostringstream strRep;
  strRep << "It's a Rectangle at x: " << getX() << ", y: " << getY()
         << " with width: " << width << " and height: " << height;
  return strRep.str();
}

Circle::Circle() : Shape() {
  this->radius = 0;
}

Circle::Circle(int r) : Shape() {
  this->radius = r;
}

Circle::Circle(int x, int y, int r) : Shape(x, y) {
  this->radius = r;
}

Circle::~Circle() {
}

Circle* Circle::copy() {
  return new Circle(*this);
}

int Circle::getRadius() const {
  return radius;
}

void Circle::setRadius(int r) {
  this->radius = r;
}

string Circle::as_string() const {
  ostringstream strRep;
  strRep << "It's a Circle at x: " << getX() << ", y: " << getY()
         << ", radius: " << radius;
  return strRep.str();
}

RightTriangle::RightTriangle() : Shape() {
  this->base = 0;
  this->height = 0;
}

RightTriangle::RightTriangle(int b, int h) : Shape() {
  this->base = b;
  this->height = h;
}

RightTriangle::RightTriangle(int x, int y, int b, int h) : Shape(x, y) {
  this->base = b;
  this->height = h;
}
RightTriangle::~RightTriangle() {
}
RightTriangle* RightTriangle::copy() {
  return new RightTriangle(*this);
}

int RightTriangle::getBase() const {
  return base;
}

int RightTriangle::getHeight() const {
  return height;
}

void RightTriangle::setBase(int b) {
  this->base = b;
}

void RightTriangle::setHeight(int h) {
  this->height = h;
}

string RightTriangle::as_string() const {
  ostringstream strRep;
  strRep << "It's a Right Triangle at x: " << getX() << ", y: " << getY()
         << " with base: " << base << " and height: " << height;
  return strRep.str();
}
