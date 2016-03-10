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

#include <KiwiGraphics/KiwiPoint.hpp>
#include <KiwiGraphics/KiwiAffineMatrix.hpp>


namespace kiwi
{
    namespace graphics
    {
        // ================================================================================ //
        //                                      PATH                                        //
        // ================================================================================ //
        
        //! @brief The path holds a set of points.
        //! @details The path is used to represent a set of curves and shapes. It can be discontinuous.
        //! @todo Check if constructors with shapes are needed
        //! @todo Check if withdrawn functions are needed (addArc, addPieChart, addArc, overlaps)
        //! @todo Check if bounds are needed and how they can be implemented (dizcretisation, recompute at every change)
        class Path
        {
        public:
            //! @brief Default constructor
            //! @details Initializes a path positioned at the origin
            Path() noexcept {addNode(Node(Point(), Move));};
            
            //! @brief Copy constructor
            //! @details Copies all the nodes of a path
            Path(Path const& path) noexcept : m_nodes(path.m_nodes) {};
            
            //! @brief Move constructor
            //! @details Moves the nodes to the new constructed path
            Path(Path&& other) noexcept : m_nodes(std::move(other.m_nodes)){};
            
            //! @brief Position constructor
            //! @details Initializes a constructor at a certain position
            explicit Path(Point const& position) noexcept {addNode(Node(position, Move));}
            
            //! @brief Assignment operator
            //! @details Clears all nodes and copies all nodes from other
            Path& operator=(Path const& other) noexcept;
            
            //! @brief Move assignment operator
            //! @details Moves the nodes from other
            Path& operator=(Path&& other) noexcept;
            
            //! @brief Destructor
            ~Path() noexcept = default;
            
            //! @brief Gets the number of nodes
            inline ulong size() const noexcept {return static_cast<ulong>(m_nodes.size());}
            
            //! @brief Returns true if the path is empty
            inline bool empty() const noexcept {return m_nodes.size() <= 1;}
            
            //! @brief Removes all nodes
            //! @details After clear, the path continues drawing from origin
            void clear() noexcept;
            
            //! @brief Applies an affine transform to the path
            //! @details AffineTransform is applied to all nodes
            void transform(AffineMatrix const& matrix) noexcept;
            
            //! @brief Gets a affine transformed path
            //! @details Retrieves a copy, doesn't apply changes to path
            Path transformed(AffineMatrix const& matrix) const noexcept;
            
            //! @brief Moves drawing cursor to the position
            //! @details Doesn't actually move the nodes, sets the next drawing position instead. Paths can be discontinuous.
            void moveTo(Point const& position) noexcept;

            //! @brief Adds a line to the current path
            //! @param line_edge Draws a line from cursor to line_edge
            void addLine(Point const& line_edge) noexcept;
            
            //! @brief Adds a sets of line
            //! @param il Contains the edges position of the added lines
            void addLines(std::initializer_list<Point> il) noexcept;
            
            //! @brief Adds a quadradic bezier curve
            void addQuadratic(Point const& control, Point const& end) noexcept;
            
            //! @brief Adds a set of quadratic bezier curves
            //! @param il contains the control positions for every quadradic curve
            void addQuadratics(std::initializer_list<std::array<Point, 2>> il) noexcept;
            
            //! @brief Adds a cubic curve to the path
            void addCubic(Point const& control1, Point const& control2, Point const& end) noexcept;
            
            //! @brief Adds a set of cubic bezier curves
            //! @param il Contains the control positions for every cubic curve
            void addCubics(std::initializer_list<std::array<Point, 3>> il) noexcept;
            
            //! @brief Adds rectangle to the path
            //! @param top_left The top_left corner position of the added rectangle
            //! @param width Horizontal width of the rectangle
            //! @param height Vertical height of the the rectangle
            void addRectangle(Point const& top_left, const double width, const double height) noexcept;
            
            //! @brief Add an ellipse to the path
            //! @param center Center of the ellipse
            //! @param radius_x Horizontal radius
            //! @param radius_y Vertical radius
            void addEllipse(Point const& center, const double radius_x, const double radius_y) noexcept;
            
            //! @brief Appends a path
            void addPath(Path const& path) noexcept;
            
            //! @brief Closes the final edge of the path
            //! @details If the path is discontinuous, only the final part of the path is closed
            void close() noexcept;
            
        private:
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
                constexpr Node(Point const& pos, const Mode mode) noexcept : m_pos(pos), m_mode(mode){};
                
                constexpr Node(Node const& other) noexcept : m_pos(other.m_pos), m_mode(other.m_mode){};
                Node& operator=(Node const& other) noexcept;
                
                ~Node() noexcept = default;
                
                constexpr inline Point getPos() const noexcept{return m_pos;}
                constexpr inline Mode getMode() const noexcept{return m_mode;}
                
                inline void setPos(const Point pos) noexcept {m_pos = pos;};
                
                void transform(AffineMatrix const& matrix) noexcept {matrix.applyTo(m_pos);}
                
                Node() = delete;
                
            private:
                Point m_pos;
                Mode  m_mode;
            };
            
            //! @internal
            bool haveSamePos(Node const& node_l, Node const& node_r) const noexcept;
            
            std::vector<Node> m_nodes;
            
            //! @internal
            Node lastMoveNode() const noexcept;
            
            //! @internal
            void addNode(Node const& node) noexcept{m_nodes.push_back(node);}
        };
    }
}

#endif // KIWI_PATH_H_INCLUDED
