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

#include <KiwiGraphics/KiwiPath.hpp>

namespace kiwi
{
    namespace graphics
    {
        
        // =================================
        // Node
        // =================================
        
        Path::Node& Path::Node::operator=(Node const& other) noexcept
        {
            if (this != &other)
            {
                m_pos = other.m_pos;
                m_mode = other.m_mode;
            }
            
            return *this;
        }
        
        bool Path::haveSamePos(Node const& node_l, Node const& node_r) const noexcept
        {
            Point pos_left(node_l.getPos());
            Point pos_right(node_r.getPos());
            
            return pos_left.x() == pos_right.x() && pos_left.y() == pos_right.y();
        }
        
        // =================================
        // Path
        // =================================
        
        inline Path& Path::operator=(Path const& other) noexcept
        {
            if (this != &other)
            {
                m_nodes = other.m_nodes;
            }
            return *this;
        }
        
        inline Path& Path::operator=(Path&& other) noexcept
        {
            if (this != &other)
            {
                m_nodes = std::move(other.m_nodes);
            }
            return *this;
        }
        
        void Path::moveTo(Point const& point) noexcept
        {
            if(m_nodes.back().getMode() == Move)
            {
                m_nodes.back().setPos(point);
            }
            else
            {
                addNode(Node(point, Move));
            }
        }
        
        void Path::lineTo(Point const& point) noexcept
        {
            addNode(Node(point, Linear));
        }
        
        void Path::addLines(std::initializer_list<Point> il) noexcept
        {
            for (auto point : il)
            {
                lineTo(point);
            }
        }
        
        void Path::quadraticTo(Point const& control, Point const& end) noexcept
        {
            addNode(Node(control, Quadratic));
            addNode(Node(end, Quadratic));
        }
        
        void Path::addQuadratics(std::initializer_list<std::array<Point, 2>> il) noexcept
        {
            for (auto quad_curve = il.begin(); quad_curve != il.end(); ++quad_curve)
            {
                quadraticTo((*quad_curve)[0], (*quad_curve)[1]);
            }
        }
        
        void Path::cubicTo(Point const& control1, Point const& control2, Point const& end) noexcept
        {
            addNode(Node(control1, Cubic));
            addNode(Node(control2, Cubic));
            addNode(Node(end, Cubic));
        }
        
        void Path::addCubics(std::initializer_list<std::array<Point, 3>> il) noexcept
        {
            for (auto cubic_curve = il.begin(); cubic_curve != il.end(); ++cubic_curve)
            {
                cubicTo((*cubic_curve)[0], (*cubic_curve)[1], (*cubic_curve)[2]);
            }
        }
        
        void Path::addRectangle(Point const& top_left, const double width, const double height) noexcept
        {
            moveTo(top_left);
            lineTo(top_left + Point(width, 0));
            lineTo(top_left + Point(width, height));
            lineTo(top_left + Point(0, height));
            close();
        }
        
        void Path::addEllipse(Point const& center, const double radius_x, const double radius_y) noexcept
        {
            const Point delta(std::abs(radius_x), std::abs(radius_y));
            const Point top_left = center - delta;
            
            const double hw = radius_x;
            const double hw55 = hw * 0.55f;
            const double hh = radius_y * 0.5f;
            const double hh55 = hh * 0.55f;
            const double cx = top_left.x() + hw;
            const double cy = top_left.y() + hh;
            
            moveTo(Point(cx, cy - hh));
            cubicTo(Point(cx + hw55, cy - hh), Point(cx + hw, cy - hh55), Point(cx + hw, cy));
            cubicTo(Point(cx + hw, cy + hh55), Point(cx + hw55, cy + hh), Point(cx, cy + hh));
            cubicTo(Point(cx - hw55, cy + hh), Point(cx - hw, cy + hh55), Point(cx - hw, cy));
            cubicTo(Point(cx - hw, cy - hh55), Point(cx - hw55, cy - hh), Point(cx, cy - hh));
            close();
        }
        
        void Path::addPath(Path const& other) noexcept
        {
            for(auto node : other.m_nodes)
            {
                addNode(node);
            }
        }
        
        void Path::clear() noexcept
        {
            m_nodes.clear();
            m_nodes.emplace_back(Point(0, 0), Move);
        }
        
        void Path::close() noexcept
        {
            const Node last_move = lastMoveNode();
            
            if(!haveSamePos(last_move, m_nodes.back()))
            {
                addNode(Node(last_move.getPos(), Linear));
            }
            
            addNode(Node(last_move.getPos(), Close));
        }
        
        Path::Node Path::lastMoveNode() const noexcept
        {
            //! By construction Path::m_nodes is never empty
            assert(m_nodes.size() > 0);
            
            auto isMoveNode = [](Node const& current_node){return current_node.getMode() == Move;};
            return *std::find_if(m_nodes.rend(), m_nodes.rbegin(), isMoveNode);
        }
        
        void Path::transform(AffineMatrix const& matrix) noexcept
        {
            for(auto node : m_nodes)
            {
                node.transform(matrix);
            }
        }
        
        Path Path::transformed(AffineMatrix const& matrix) const noexcept
        {
            Path p(*this);
            p.transform(matrix);
            return p;
        }
    }
}