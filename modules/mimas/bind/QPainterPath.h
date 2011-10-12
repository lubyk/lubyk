namespace mimas {

/** This is a dummy class to create bindings for objects inheriting from Qt's
 * QPainterPath.
 *
 * @see Path
 */
class QPainterPath {
 public:
  void moveTo(float x, float y);

  /** Bezier curve from current point to endPoint with control points c1, c2.
   */
  void cubicTo(float c1X, float c1Y, float c2X, float c2Y, float endPointX, float endPointY);

  void addRect(float x, float y, float w, float h);

  void lineTo(float x, float y);
};

} // mimas

