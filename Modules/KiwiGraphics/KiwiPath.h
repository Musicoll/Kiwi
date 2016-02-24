/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_PATH_H_INCLUDED
#define KIWI_PATH_H_INCLUDED

#include <KiwiGraphics/KiwiRectangle.h>
#include <assert.h>

/**
 @Todo : (dashed line support, other shapes ?)
 */
namespace kiwi
{
    namespace graphics
    {
        // ================================================================================ //
        //                                      PATH                                        //
        // ================================================================================ //
        
        class Sketch;
        
        //! The path holds a set of points.
        /**
         The point is used to represent a set a segment in a space and allows several modification.
         */
        class Path
        {
        public:
            
            /** The graphic behavior of the joint between lines.
             @see EndCapMode
             */
            enum Joint
            {
                Mitered,    ///< mitered corners.
                Curved,     ///< rounded corners.
                Beveled     ///< beveled corners.
            };
            
            /** The graphic behavior of the ends of lines.
             @see JointMode
             */
            enum LineCap
            {
                Butt,       ///< flat and stopped to end point.
                Square,     ///< flat but exeeding the end point for half the line thickness.
                Round       ///< round ends of lines.
            };
            
            //! Constructor.
            /** The function initializes an empty path.
             */
            inline Path() noexcept {addNode(Node(Point(), Move));};
            
            //! Constructor.
            /** The function initializes a path with another.
             @param path The other path.
             */
            inline Path(Path const& path) noexcept : m_nodes(path.m_nodes), m_bounds(path.m_bounds) {}
            
            //! Constructor.
            /** The function initializes a path with another.
             @param path The other path.
             */
            inline Path(Path&& path) noexcept {m_nodes.swap(path.m_nodes); std::swap(m_bounds, path.m_bounds);}
            
            //! Constructor.
            /** The function initializes a path with an origin.
             @param path The other path.
             */
            inline Path(Point const& pt) noexcept {addNode(Node(pt, Move));}
            
            //! Constructor.
            /** The function initializes a path with a first point.
             @param path The other path.
             */
            inline Path(Point&& pt) noexcept {addNode(Node(std::forward<Point>(pt), Move));}
            
            //! Linear constructor.
            /** The function initializes a path with a segment.
             @param segment The segment.
             */
            inline Path(Segment const& segment)
            {
                moveTo(segment.start());
                addNode(Node(segment.end(), Linear));
            }
            
            //! Quadratic constructor.
            /** The function initializes a path with a quadratic bezier curve.
             @param curve The quadratic bezier curve.
             */
            inline Path(BezierQuad const& curve)
            {
                moveTo(curve.start());
                addNode(Node(curve.controlPoint(), Quadratic));
                addNode(Node(curve.end(), Quadratic));
            }
            
            //! Cubic constructor.
            /** The function initializes a path with a cubic bezier curve.
             @param curve The cubic bezier curve.
             */
            inline Path(BezierCubic const& curve)
            {
                moveTo(curve.start());
                addNode(Node(curve.controlPoint1(), Quadratic));
                addNode(Node(curve.controlPoint2(), Quadratic));
                addNode(Node(curve.end(), Quadratic));
            }
            
            //! Linear constructor.
            /** The function initializes a path with a line.
             @param start The first point of the line.
             @param end   The end point of the line.
             */
            inline static Path line(Point const& start, Point const& end)
            {
                Path path(start);
                path.addNode(Node(end, Linear));
                return path;
            }
            
            //! Linear constructor.
            /** The function initializes a path with a line.
             @param start The first point of the line.
             @param end   The end point of the line.
             */
            inline static Path line(Point&& start, Point&& end)
            {
                Path path(std::forward<Point>(start));
                path.addNode(Node(std::forward<Point>(end), Linear));
                return path;
            }
            
            //! Linear constructor.
            /** The function initializes a path with a set of lines. The number of points must be superior or equal to 2.
             @param il The set of points.
             */
            inline static Path lines(std::initializer_list<Point> il)
            {
                assert(il.size() < 2 && "The number of points must be superior or equal to 2 to create lines.");
                auto it = il.begin();
                Path path(*it++);
                path.addNodes(il.size() - 1, it, il.end(), Linear);
                return path;
            }
            
            //! Quadratic constructor.
            /** The function initializes a path with a quadratic bezier curve.
             @param start The first point of the curve.
             @param control The control point curve.
             @param end   The end point of the curve.
             */
            inline static Path quadratic(Point const& start, Point const& control, Point const& end)
            {
                Path path(start);
                path.addNode(Node(control, Quadratic));
                path.addNode(Node(end, Quadratic));
                return path;
            }
            
            //! Quadratic constructor.
            /** The function initializes a path with a quadratic bezier curve.
             @param start The first point of the curve.
             @param control The control point curve.
             @param end   The end point of the curve.
             */
            inline static Path quadratic(Point&& start, Point&& control, Point&& end)
            {
                Path path(std::forward<Point>(start));
                path.addNode(Node(std::forward<Point>(control), Quadratic));
                path.addNode(Node(std::forward<Point>(end), Quadratic));
                return path;
            }
            
            //! Quadratic constructor.
            /** The function initializes a path with a set of quadratic bezier curves. The number of points must be odd and superior or equal to 3.
             @param il The set of points.
             */
            inline static Path quadratics(std::initializer_list<Point> il)
            {
                assert(il.size() < 3 && "The number of points must be superior or equal to 3 to create quadratic bezier curves.");
                assert((il.size() % 3) != 1 && "The number of points must be odd to create quadratic bezier curves.");
                auto it = il.begin();
                Path path(*it++);
                path.addNodes(il.size() - 1, it, il.end(), Quadratic);
                return path;
            }
            
            //! Cubic constructor.
            /** The function initializes a path with a cubic bezier curve.
             @param start The first point of the curve.
             @param control1 The first control point curve.
             @param control2 The second control point curve.
             @param end   The end point of the curve.
             */
            inline static Path cubic(Point const& start, Point const& control1, Point const& control2, Point const& end)
            {
                Path path(start);
                path.addNode(Node(control1, Cubic));
                path.addNode(Node(control2, Cubic));
                path.addNode(Node(end, Cubic));
                return path;
            }
            
            //! Cubic constructor.
            /** The function initializes a path with a cubic bezier curve.
             @param start The first point of the curve.
             @param control1 The first control point curve.
             @param control2 The second control point curve.
             @param end   The end point of the curve.
             */
            inline static Path cubic(Point&& start, Point&& control1, Point&& control2, Point&& end)
            {
                Path path(std::forward<Point>(start));
                path.addNode(Node(std::forward<Point>(control1), Cubic));
                path.addNode(Node(std::forward<Point>(control2), Cubic));
                path.addNode(Node(std::forward<Point>(end), Cubic));
                return path;
            }
            
            //! Cubic constructor.
            /** The function initializes a path with a set of cubic bezier curves. The number of points must be a multiple of 3 + 1 and superior or equal to 4.
             @param il The set of points.
             */
            inline static Path cubics(std::initializer_list<Point> il)
            {
                assert(il.size() < 3 && "The number of points must be superior or equal to 4 to create cubic bezier curves.");
                assert(!((il.size() - 1) % 3) && "The number of points must be a multiple of 3 + 1 to create cubic bezier curves.");
                auto it = il.begin();
                Path path(*it++);
                path.addNodes(il.size() - 1, it, il.end(), Cubic);
                return path;
            }
            
            //! Sets the path with another path.
            /** The function the path with another path.
             @param other The other path.
             @return The path.
             */
            inline Path& operator=(Path const& other) noexcept
            {
                m_nodes = other.m_nodes;
                m_bounds = other.m_bounds;
                return *this;
            }
            
            //! Sets the path with another path.
            /** The function the path with another path.
             @param other The other path.
             @return The path.
             */
            inline Path& operator=(Path&& other) noexcept
            {
                std::swap(m_nodes, other.m_nodes);
                std::swap(m_bounds, other.m_bounds);
                return *this;
            }
            
            //! Destructor.
            /** The function deletes the path.
             */
            inline ~Path() noexcept {clear();}
            
            //! Retrieves the number of nodes of the path.
            /** The function retrieves the number of nodes of the path.
             @return The number of nodes of the path.
             */
            inline ulong size() const noexcept {return (ulong)m_nodes.size(); }
            
            //! Retrieves if the path is empty.
            /** The function retrieves if the path is empty.
             @return True if the path is empty, otherwise false.
             */
            inline bool empty() const noexcept {return m_nodes.empty();}
            
            //! Clears the path.
            /** The function clears a point to the path.
             */
            inline void clear() noexcept {m_nodes.clear(); m_bounds = Rectangle();};
            
            //! Retrieves the bounds of the path.
            /** The function retrieves the bounds of the path. The bounds rectangle is the smallest rectangle that contains all the points.
             @return The bounds of the path.
             */
            inline Rectangle bounds() const noexcept {return m_bounds;};
            
            //! Apply a 2D affine transformation to the path.
            /** The function applies a 2D affine transformation to the path.
             @param matrix The affine matrix to apply to.
             */
            void transform(AffineMatrix const& matrix) noexcept;
            
            //! Apply a 2D affine transformation to the path.
            /** The function applies a 2D affine transformation to the path.
             @param matrix The affine matrix to apply to.
             */
            Path transformed(AffineMatrix const& matrix) const noexcept;
            
            //! Adds a new point to the path not linked with the previous one.
            /** The function adds a new point to the path that won't be linked to the previous node.
             @param point The point to add.
             */
            inline void moveTo(Point const& point) noexcept
            {
                if(!empty() && m_nodes[m_nodes.size() - 1].mode() == Move)
                {
                    addNode(m_nodes.size() - 1, Node(point, Move));
                }
                else
                {
                    addNode(Node(point, Move));
                }
            }
            
            //! Adds a new point to the path not linked with the previous one.
            /** The function adds a new point to the path that won't be linked to the previous node.
             @param point The point to add.
             */
            inline void moveTo(Point&& point) noexcept
            {
                if(!empty() && m_nodes[m_nodes.size() - 1].mode() == Move)
                {
                    addNode(m_nodes.size() - 1, Node(std::forward<Point>(point), Move));
                }
                else
                {
                    addNode(Node(std::forward<Point>(point), Move));
                }
            }
            
            //! Adds a the point that will be linked to the previous point linearly.
            /** The function adds a point to the path that will be linked to the previous node linearly.
             @param point The point to add.
             */
            inline void lineTo(Point const& point) noexcept
            {
                addNode(Node(point, Linear));
            }
            
            //! Adds a point that will be linked to the previous point linearly.
            /** The function adds a point to the path that will be linked to the previous node linearly.
             @param point The point to add.
             */
            inline void lineTo(Point&& point) noexcept
            {
                addNode(Node(std::forward<Point>(point), Linear));
            }
            
            //! Add a set of points to that will be linked linearly.
            /** The function adds a set of points to that will be linked linearly.
             @param il The set of points.
             */
            inline void lineTo(std::initializer_list<Point> il) noexcept
            {
                addNodes(il, Linear);
            }
            
            //! Adds a quadratic bezier curve to the path.
            /** The function adds a quadratic bezier curve to the path.
             @param control The control point.
             @param end     The end point.
             */
            inline void quadraticTo(Point const& control, Point const& end) noexcept
            {
                addNode(Node(control, Quadratic));
                addNode(Node(end, Quadratic));
            }
            
            //! Adds a quadratic bezier curve to the path.
            /** The function adds a quadratic bezier curve to the path.
             @param control The control point.
             @param end     The end point.
             */
            inline void quadraticTo(Point&& control, Point&& end) noexcept
            {
                addNode(Node(std::forward<Point>(control), Quadratic));
                addNode(Node(std::forward<Point>(end), Quadratic));
            }
            
            //! Add a set of quadratic bezier curves to the path.
            /** The function adds a set of quadratic bezier curves to the path. The number of point must be even, the first point of each pair defines the control point and the second point of each pair defines the end point.
             @param il The set of points that represent the quadratic bezier curves.
             */
            inline void quadraticsTo(std::initializer_list<Point> il) noexcept
            {
                assert(!(il.size() % 2) && "Quadractic bezier curve must have an even number of points.");
                addNodes(il, Quadratic);
            }
            
            //! Adds a cubic bezier curve to the path.
            /** The function adds a cubic bezier curve to the path.
             @param control1 The first control point.
             @param control2 The seconf control point.
             @param end     The end point.
             */
            inline void cubicTo(Point const& control1, Point const& control2, Point const& end) noexcept
            {
                addNode(Node(control1, Cubic));
                addNode(Node(control2, Cubic));
                addNode(Node(end, Cubic));
            }
            
            //! Adds a cubic bezier curve to the path.
            /** The function adds a cubic bezier curve to the path.
             @param control1 The first control point.
             @param control2 The seconf control point.
             @param end     The end point.
             */
            inline void cubicTo(Point&& control1, Point&& control2, Point&& end) noexcept
            {
                addNode(Node(std::forward<Point>(control1), Cubic));
                addNode(Node(std::forward<Point>(control2), Cubic));
                addNode(Node(std::forward<Point>(end), Cubic));
            }
            
            //! Add a set of quadratic bezier curves to the path.
            /** The function adds a set of quadratic bezier curves to the path. The number of point must be a multiple of 3, the first point of each set defines the first control point, the second point defines the second control point and the third point defines the end point.
             @param il The set of points that represent the quadratic bezier curves.
             */
            inline void cubicsTo(std::initializer_list<Point> il) noexcept
            {
                assert(!(il.size() % 3) && "Cubic bezier curve must have a number of points multiple of 3.");
                addNodes(il, Cubic);
            }
            
            //! Add another path to the path.
            /** The function adds another path to the path.
             @param rect The rectangle.
             */
            inline void addPath(Path const& path) noexcept
            {
                path.size();
                for(Node node : path.m_nodes)
                {
                    addNode(node);
                }
            }
            
            //! Add rectangle to the path.
            /** The function adds rectangle to the path.
             @param rect The rectangle.
             */
            inline void addRectangle(Rectangle const& rect) noexcept
            {
                moveTo(rect.position());
                addNode(Node(Point(rect.right(), rect.y()), Linear));
                addNode(Node(Point(rect.right(), rect.bottom()), Linear));
                addNode(Node(Point(rect.x(), rect.bottom()), Linear));
                close();
            }
            
            //! Add a rounded rectangle to the path.
            /** The function adds a rounded rectangle to the path.
             @param rect The rectangle.
             @param roundness The roundness of rectangle's corners
             */
            void addRectangle(Rectangle const& rect, double roundness) noexcept;
            
            //! Add a rounded rectangle to the path.
            /** The function adds a rounded rectangle to the path specifying wich corner to round.
             @param x  The rectangle's x.
             @param y  The rectangle's y.
             @param w  The rectangle's width.
             @param h  The rectangle's height.
             @param rx The x corner size.
             @param ry The y corner size.
             @param ry The y corner size.
             @param corner A flag specifying wich corner to round as defined by the #Rectangle::Corner Enum
             */
            void addRectangle(const double x, const double y, const double w, const double h, double rx, double ry, const char corner) noexcept;
            
            //! Add an ellipse to the path.
            /** The function adds an ellipse to the path.
             @param rect The bound rectangle of the ellipse.
             */
            void addEllipse(Rectangle const& rect) noexcept;
            
            //! Add an ellipse to the path.
            /** The function adds an ellipse to the path.
             @param center The center of the ellipse.
             @param rx The x radius.
             @param rx The y radius.
             */
            void addEllipse(Point const& center, const double rx, const double ry) noexcept;
            
            //! Add an elliptical arc to the path.
            /** The function adds an elliptical arc to the path.
             @param center      The center of the arc.
             @param radius      The radius of the arc.
             @param startAngle  The start angle.
             @param endAngle    The end angle.
             */
            void addArc(Point const& center, const Point& radius, const double startAngle, const double endAngle) noexcept;
            
            //! Add an elliptical arc to the path.
            /** The function adds an elliptical arc to the path.
             @param center      The center of the arc.
             @param radius      The radius of the arc.
             @param startAngle  The start angle.
             @param endAngle    The end angle.
             @param rotAngle    The angle of rotation of the ellipse.
             */
            void addArc(Point const& center, const Point& radius, const double startAngle, const double endAngle, const double rotAngle) noexcept;
            
            //! Add a "pie-chart" shape to the path.
            /** The function adds a "pie-chart" shape to the path.
             @param center      The center of the arc.
             @param radius      The radius of the arc.
             @param startAngle  The start angle.
             @param endAngle    The end angle.
             */
            void addPieChart(Point const& center, const Point& radius, const double startAngle, const double endAngle) noexcept;
            
            //! Add a node to close the path.
            /** The function adds a node to close the path.
             */
            inline void close() noexcept
            {
                if(!empty())
                {
                    const Point lastMove = lastMovePoint();
                    if(m_nodes[size() - 1] != lastMove)
                    {
                        addNode(Node(lastMove, Linear));
                    }
                    addNode(Node(lastMove, Close));
                }
            }
            
            bool overlaps(Rectangle const& rect) const noexcept;
            
        private:
            friend class Sketch;
            
            //! @internal
            enum Mode
            {
                Close       = 0,
                Move        = 1,
                Linear      = 2,
                Quadratic   = 3,
                Cubic       = 4
            };
            
            //! @internal
            class Node
            {
            public:
                constexpr inline Node() noexcept : data{0., 0.}, m(Linear) {};
                constexpr inline Node(Point const& pt, Mode const _m = Linear) noexcept : data{pt.m_data[0], pt.m_data[1]}, m(_m) {};
                constexpr inline Node(Node const& other) noexcept : data{other.data[0], other.data[1]}, m(other.m) {};
                inline Node(Point&& pt, Mode const _m = Linear) noexcept : m(_m) {std::swap(data, pt.m_data);};
                inline Node(Node&& other) noexcept : m(other.m) {std::swap(data, other.data);};
                inline ~Node() noexcept {};
                inline Node& operator=(Point const& pt) noexcept {memcpy(data, pt.m_data, sizeof(double)*2); return *this;}
                inline Node& operator=(Node const& other) noexcept {memcpy(data, other.data, sizeof(double)*2); m = other.m; return *this;}
                inline Node& operator=(Point&& pt) noexcept {std::swap(data, pt.m_data); return *this;}
                inline Node& operator=(Node&& other) noexcept {std::swap(data, other.data); m = other.m; return *this;}
                inline bool operator!=(Point const& other) const noexcept {return (data[0] != other.x() && data[1] != other.y());}
                inline Point point() const noexcept {return Point(data[0], data[1]);}
                inline Mode mode() const noexcept{return m;}
                inline void transform(AffineMatrix const& matrix) noexcept {matrix.applyTo(data[0], data[1]);}
                
            private:
                double data[2];
                Mode   m;
            };
            
            std::vector<Node> m_nodes;
            Rectangle    m_bounds;
            
            //! @internal
            inline void rebound(Node const& newnode) noexcept
            {
                if(m_nodes.size() > 1)
                {
                    const Point pt = newnode.point();
                    if(pt.x() < m_bounds.x()) m_bounds.left(pt.x());
                        else if(pt.x() > m_bounds.right()) m_bounds.right(pt.x());
                            if(pt.y() < m_bounds.y()) m_bounds.top(pt.y());
                                else if(pt.y() > m_bounds.bottom()) m_bounds.bottom(pt.y());
                                    }
                else
                {
                    m_bounds.position(m_nodes[0].point());
                    m_bounds.size(Size());
                }
            }
            
            //@internal
            inline void resize(const std::vector<Node>::size_type size) noexcept
            {
                m_nodes.resize(size);
            }
            
            //@internal
            inline Point lastMovePoint() noexcept
            {
                for(auto it = m_nodes.rbegin(); it != m_nodes.rend(); ++it)
                {
                    if((*it).mode() == Move)
                        return (*it).point();
                        }
                return Point();
            }
            
            //@internal
            inline void addNodes(std::initializer_list<Point> il, Mode mode) noexcept
            {
                std::vector<Node>::size_type size = m_nodes.size();
                resize(size + il.size());
                for(auto it = il.begin(); it != il.end(); ++it){
                    addNode(size++, Node((*it), mode));
                }
            }
            
            //@internal
            inline void addNodes(const std::vector<Point>::size_type s, const Point* begin, const Point* end, Mode mode) noexcept
            {
                std::vector<Node>::size_type size = m_nodes.size();
                resize(size + s);
                for(auto it = begin; it != end; ++it){
                    addNode(size++, Node((*it), mode));
                }
            }
            
            //@internal
            inline void addNode(Node const& node) noexcept
            {
                m_nodes.push_back(node);
                rebound(node);
            }
            
            //@internal
            inline void addNode(Node&& node) noexcept
            {
                m_nodes.push_back(std::forward<Node>(node));
                rebound(m_nodes[m_nodes.size() - 1]);
            }
            
            //@internal
            inline void addNode(const std::vector<Node>::size_type pos, Node const& node) noexcept
            {
                m_nodes[pos] = node;
                rebound(node);
            }
            
            //@internal
            inline void addNode(const std::vector<Node>::size_type pos, Node&& node) noexcept
            {
                m_nodes[pos] = std::forward<Node>(node);
                rebound(m_nodes[pos]);
            }
        };
    }
}

#endif // KIWI_PATH_H_INCLUDED
