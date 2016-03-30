/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris
 
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

#include <KiwiGraphics/KiwiPoint.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"

namespace kiwi
{
    bool areNear(gui::Point const& pt_left, gui::Point const& pt_right)
    {
        return areNear(pt_left, pt_right, 1e-12);
    }
    
    TEST_CASE("Point", "[Point]")
    {
        SECTION("Constructors, Getters, Setters")
        {
            gui::Point pt_default;
            gui::Point pt_double(2.5, 3.);
            gui::Point pt_copy(pt_double);
            
            CHECK((pt_default.x() == 0 && pt_default.y() == 0));
            CHECK((pt_double.x() == 2.5 && pt_double.y() == 3.));
            CHECK((pt_copy.x() == pt_double.x() && pt_copy.y() == pt_double.y()));
            
            pt_copy.x(13.);
            pt_copy.y(12.5);
            
            CHECK((pt_copy.x() == 13. && pt_copy.y() == 12.5));
        }
        SECTION("areNear")
        {
            const gui::Point pt(1., 1.);
            
            //areNear standard
            {
                double standard_eps = 1e-12;
                
                const gui::Point pt_close(pt.x(), pt.y() + standard_eps - std::numeric_limits<double>::epsilon());
                const gui::Point pt_not_close(pt.x(),
                                         pt.y() + standard_eps + std::numeric_limits<double>::epsilon());
                
                CHECK(areNear(pt, pt_close));
                CHECK_FALSE(areNear(pt, pt_not_close));
            }
            //areNear with epsilon redefined
            {
                const double eps = 1e-3;
                auto are_equal = [eps](gui::Point const& l_pt, gui::Point const& r_pt){return areNear(l_pt, r_pt, eps);};
                
                const gui::Point pt_close(pt.x(), pt.y() + eps - std::numeric_limits<double>::epsilon());
                const gui::Point pt_not_close(pt.x(),
                                         pt.y() + eps + std::numeric_limits<double>::epsilon());
                
                CHECK(are_equal(pt, pt_close));
                CHECK_FALSE(are_equal(pt, pt_not_close));
            }
        }
        SECTION("Operators")
        {
            const gui::Point p1(3, 3);
            const gui::Point p2(-1, 2);
            const gui::Point p3(4.5, -5.);
            const gui::Point p4(-1, -3.2);
            
            //Operator=
            {
                gui::Point pt;
                pt = p1;
                CHECK(areNear(pt, p1));
            }
            
            //Operator+ && Operator+=
            {
                CHECK(areNear(p1 + p2, gui::Point(3 - 1, 3 + 2)));
                CHECK(areNear(p3 + p4, gui::Point(4.5 - 1, -5 - 3.2)));
                
                gui::Point pt(12.6, -11);
                gui::Point pt_bckup = pt;
                pt+=p2;
                
                CHECK(areNear(pt, p2 + pt_bckup));
            }
            
            //Operator- && Operator-=
            {
                
                CHECK(areNear(-p3, gui::Point(-4.5, 5.)));
                CHECK(areNear(p1 - p2, gui::Point(3 + 1, 3 - 2)));
                CHECK(areNear(p4 - p3, gui::Point(-1 - 4.5, -3.2 + 5)));
                
                gui::Point pt(10.6, 12.3);
                gui::Point pt_bckup = pt;
                pt-= p1;
                CHECK(areNear(pt, gui::Point(10.6 - 3, 12.3 - 3)));
                CHECK(areNear(pt, pt_bckup - p1));
            }
            
            //Operator* && Operator*=
            {
                CHECK(areNear(3 * p4, gui::Point(3 * (-1), 3 * (-3.2))));
                CHECK(areNear(p4 * 3, 3 * p4));
                
                gui::Point pt(1.5, -2);
                gui::Point pt_bckup = pt;
                pt*=5;
                CHECK(areNear(pt, gui::Point(1.5 * 5, -2 * 5)));
                CHECK(areNear(pt, pt_bckup * 5));
            }
            
            //Operator/ && Operator/=
            {
                CHECK(areNear(p3 / 2, gui::Point(4.5 / 2, -5. / 2)));
                
                gui::Point pt(-1.2, 3.2);
                gui::Point pt_bckup;
                pt/=3.;
                CHECK(areNear(pt, gui::Point(-1.2 / 3, 3.2 / 3)));
            }
            
            SECTION("Scalar product, Norm")
            {
                gui::Point p1(1.3, -2.5);
                gui::Point p2(-2, 3.4);
                
                // Scalar product
                CHECK(p1.dot(p2) == 1.3 * (-2) + -2.5 * 3.4);
                
                // Norm
                CHECK(norm(p1) == sqrt(pow(1.3, 2) + pow(-2.5, 2)));
                CHECK(norm(p2) == sqrt(p2.dot(p2)));
                
                // Distance
                CHECK(distance(p1, p2) == norm(p2 - p1));
            }
            
            SECTION("Angles, Rotations")
            {
                const double pi = M_PI;
                const double angle_eps = 1e-12;
                
                auto angle_equal = [angle_eps](double angle_l, double angle_r)
                {
                    return std::abs(angle_l - angle_r) < angle_eps;
                };
                
                const double angle_30 = pi / 6;
                const double angle_45 = pi / 4;
                const double angle_60 = pi / 3;
                const double angle_90 = pi / 2;
                const double angle_180 = pi;
                
                const gui::Point point_0(cos(0), sin(0));
                const gui::Point point_90(cos(angle_90), sin(angle_90));
                const gui::Point point_60(cos(angle_60), sin(angle_60));
                const gui::Point point_30(cos(angle_30), sin(angle_30));
                const gui::Point point_180(cos(angle_180), sin(angle_180));
                const gui::Point point_450(cos(angle_90 + 2 * pi), sin(angle_90 + 2 * pi));
                
                // Angle computation
                CHECK(angle_equal(point_90.angle(), angle_90));
                CHECK(angle_equal(point_90.angle(), point_450.angle()));
                CHECK(angle_equal(point_0.angle(), 0.));
                
                // Angle Between and to other point
                CHECK(angle_equal(gui::Point(0, 0).angleToPoint(gui::Point(0, 10)), angle_90));
                CHECK_FALSE(angle_equal(point_0.angleToPoint(point_90), angle_90));
                CHECK(angle_equal(point_90.angle() - point_30.angle(), point_60.angle()));
                
                // Rotation
                CHECK(angle_equal(point_0.rotated(angle_90).angle(), angle_90));
                CHECK(angle_equal(point_0.rotated(-angle_30).angle(), -angle_30));
                CHECK(angle_equal(point_30.rotated(angle_60).angle(), angle_90));
                
                gui::Point to_rotate = point_30;
                to_rotate.rotate(angle_60);
                CHECK(areNear(to_rotate, point_30.rotated(angle_60)));
                
                // Rotation around other point
                const gui::Point shift(1.5, 3.2);
                const gui::Point shift_30 = point_30 + shift;
                CHECK(areNear(point_30.rotated(angle_60) + shift, shift_30.rotated(shift, angle_60)));
                
                gui::Point to_rotate_around = shift_30;
                to_rotate_around.rotate(shift, angle_60);
                CHECK(areNear(to_rotate_around, point_30.rotated(angle_60) + shift));
            }
        }
    }
}
