/*
    GNU Gama -- adjustment of geodetic networks
    Copyright (C) 2000, 2002, 2013, 2014, 2018, 2019, 2020
                  Ales Cepek <cepek@gnu.org>

    This file is part of the GNU Gama C++ library.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <cmath>
#include <cstring>
#include <fstream>
#include <string>
#include <utility>

#include <Math/Business/Core/radian.h>

#include <gnu_gama/local/gamadata.h>
#include <gnu_gama/local/network.h>
#include <gnu_gama/xml/baseparser.h>
#include <gnu_gama/xml/dataobject.h>


#include <Math/Business/Core/gon2deg.h>
#include <Math/Business/Core/intfloat.h>
#include <Parsing/Service/encoding.h>
#include <Utilities/Service/size_to.h>
#include <gnu_gama/local/cluster.h>
#include <gnu_gama/local/language.h>
#include <gnu_gama/local/observation.h>
#include <gnu_gama/xml/gkfparser.h>
#include <gnu_gama/xsd.h>

namespace
{
typedef std::pair<double, bool> DB_pair;
}

namespace GNU_gama
{
namespace local
{
namespace GKFparser
{

class GKFparserImpl : public GNU_gama::BaseParser<ParserException>
{
public:
    GKFparserImpl(GNU_gama::local::LocalNetwork& locnet);
    ~GKFparserImpl();

private:
    int characterDataHandler(const char* s, int len) final;
    int startElement(const char* cname, const char** atts) final;
    int endElement(const char* name) final;

    double implicit_stdev_direction() const { return direction_stdev_; }
    double implicit_stdev_angle() const { return angle_stdev_; }
    double implicit_stdev_zangle() const { return zenith_stdev_; }
    double implicit_stdev_azimuth() const { return azimuth_stdev_; }
    double implicit_stdev_distance(double d) const
    {
        using namespace std;
        return distance_stdev_ + distance_stdev_km_ * pow(d / 1000, distance_stdev_exp_);
    }

private:
    GNU_gama::local::LocalNetwork& m_lnet;
    GNU_gama::local::PointData& m_SB;        // point list
    GNU_gama::local::ObservationData& m_OD;  // observation list
    std::string m_description;  // network description

    enum gkf_tag
    {
        tag_unknown,
        tag_gama_xml,
        tag_network,
        tag_description,
        tag_parameters,
        tag_points_observations,
        tag_point,
        tag_obs,
        tag_cov_mat,
        tag_direction,
        tag_distance,
        tag_angle,
        tag_s_distance,
        tag_z_angle,
        tag_height_differences,
        tag_dh,
        tag_coordinates,
        tag_vectors,
        tag_vec,
        tag_azimuth
    };

    gkf_tag tag(const char* cname);

    enum gkf_state
    {
        state_error,
        state_start,
        state_gama_xml,
        state_network,
        state_description,
        state_parameters,
        state_point_obs,
        state_point,
        state_obs,
        state_obs_direction,
        state_obs_distance,
        state_obs_angle,
        state_obs_sdistance,
        state_obs_zangle,
        // xsd 0.91 state_obs_dh,
        state_obs_azimuth,
        state_obs_cov,
        state_obs_after_cov,
        state_coords,
        state_coords_point,
        state_coords_cov,
        state_coords_after_cov,
        state_hdiffs,
        state_hdiffs_dh,
        state_hdiffs_cov,
        state_hdiffs_after_cov,
        state_vectors,
        state_vectors_vec,
        state_vectors_cov,
        state_vectors_after_cov,
        state_stop
    };

    // 1.7.09 std::pair<"standard deviation", "angular value in degrees">
    std::vector<std::pair<double, bool> > m_sigma;
    int m_idim, m_iband;         // covariance matrix dim. / band
    bool m_pp_xydef, m_pp_zdef;  // process_point();
    double m_pp_x, m_pp_y, m_pp_z;
    PointID m_pp_id;
    std::string m_cov_mat_data;

    // Implicit value of stanpoint ID is set for sets of
    // directions/distances and/or angles.

    std::string standpoint_id;
    double obs_from_dh;

    StandPoint* standpoint;
    Coordinates* coordinates;
    HeightDifferences* heightdifferences;
    Vectors* vectors;

    bool check_cov_mat;

    int process_gama_xml(const char** atts);
    int process_network(const char** atts);
    int process_parameters(const char** atts);
    int process_point_obs(const char** atts);
    int process_point(const char** atts);
    int process_distance(const char** atts);
    int process_angle(const char** atts);
    int process_direction(const char** atts);
    int process_sdistance(const char** atts);
    int process_zangle(const char** atts);
    // xsd 0.91 int process_obs_dh     (const char** atts);
    int process_azimuth(const char** atts);

    int process_obs(const char** atts);
    int finish_obs();

    int process_coords(const char** atts);
    int finish_coords();
    int process_coords_point(const char** atts);

    int process_hdiffs(const char** atts);
    int finish_hdiffs();
    int process_dh(const char** atts);

    int process_cov(const char** atts);
    int finish_cov(CovMat&);
    int process_obs_cov(const char** atts)
    {
        state = state_obs_cov;
        return process_cov(atts);
    }
    int process_coords_cov(const char** atts)
    {
        state = state_coords_cov;
        return process_cov(atts);
    }
    int process_hdiffs_cov(const char** atts)
    {
        state = state_hdiffs_cov;
        return process_cov(atts);
    }
    int process_vectors_cov(const char** atts)
    {
        state = state_vectors_cov;
        return process_cov(atts);
    }

    int process_vectors(const char** atts);
    int finish_vectors();
    int process_vec(const char** atts);

    // implicit values of standard deviations are initialized in
    // GKFparser::process_point_obs(const char** atts)

    double distance_stdev_;
    double distance_stdev_km_;
    double distance_stdev_exp_;
    double direction_stdev_;
    double angle_stdev_;
    double zenith_stdev_;
    double azimuth_stdev_;
};  // class GKFparser

using namespace std;

int GKFparserImpl::characterDataHandler(const char* s, int len)
{
    if(state == state_description) { m_description += string(s, len); }
    else if(state == state_obs_cov || state == state_coords_cov || state == state_vectors_cov || state == state_hdiffs_cov)
    {
        m_cov_mat_data += std::string(s, len);
        m_cov_mat_data += '\n';
        std::cout << m_cov_mat_data << std::endl;
    }
    else
    {
        if(len == 0) return 0;
        int b = 0;
        while(b < len && isspace(s[b])) b++;
        if(b == len) return 0;

        error(T_GKF_illegal_text);
    }
    
    return 0;
}



int GKFparserImpl::startElement(const char* cname, const char** atts)
{
    const gkf_tag ntag = tag(cname);

    switch(state)
    {

    case state_start:
        switch(ntag)
        {
        case tag_gama_xml: return process_gama_xml(atts);
        default: return error(T_GKF_must_start_with_gama_xml);
        }

    case state_gama_xml:
        switch(ntag)
        {
        case tag_network: return process_network(atts);
        default: return error(T_GKF_missing_tag_network);
        }

    case state_network:
        switch(ntag)
        {
        case tag_description: return (state = state_description);
        case tag_parameters: return process_parameters(atts);
        case tag_points_observations: return process_point_obs(atts);
        default:
            return error(
                T_GKF_e01a_illegal_tag + string(cname) + T_GKF_e01b_after_tag + " <network>");
        }

    case state_point_obs:
        switch(ntag)
        {
        case tag_point: return process_point(atts);
        case tag_obs: return process_obs(atts);
        case tag_coordinates: return process_coords(atts);
        case tag_height_differences: return process_hdiffs(atts);
        case tag_vectors: return process_vectors(atts);
        default:
            return error(
                T_GKF_e01a_illegal_tag + string(cname) + T_GKF_e01b_after_tag
                + " <points-observations>");
        }

    case state_obs:
        switch(ntag)
        {
        case tag_direction: return process_direction(atts);
        case tag_distance: return process_distance(atts);
        case tag_angle: return process_angle(atts);
        case tag_s_distance: return process_sdistance(atts);
        case tag_z_angle: return process_zangle(atts);
        case tag_azimuth: return process_azimuth(atts);
        case tag_cov_mat: return process_obs_cov(atts);
        default:
            return error(T_GKF_e01a_illegal_tag + string(cname) + T_GKF_e01b_after_tag + " <obs>");
        }

    case state_coords:
        switch(ntag)
        {
        case tag_point: return process_coords_point(atts);
        case tag_cov_mat: return process_coords_cov(atts);
        default:
            return error(
                T_GKF_e01a_illegal_tag + string(cname) + T_GKF_e01b_after_tag + " <coordinates>");
        }

    case state_hdiffs:
        switch(ntag)
        {
        case tag_dh: return process_dh(atts);
        case tag_cov_mat: return process_hdiffs_cov(atts);
        default:
            return error(
                T_GKF_e01a_illegal_tag + string(cname) + T_GKF_e01b_after_tag
                + " <height-differences>");
        }

    case state_vectors:
        switch(ntag)
        {
        case tag_vec: return process_vec(atts);
        case tag_cov_mat: return process_vectors_cov(atts);
        default: return error(T_GKF_e01a_illegal_tag + string(cname) + ">");
        }

    case state_obs_after_cov:
    case state_coords_after_cov:
    case state_hdiffs_after_cov:
    case state_vectors_after_cov: { return error(T_GKF_no_observations_after_cov_mat);
    }

    default: return error(T_GKF_e01a_illegal_tag + string(cname) + ">");
    }

    return 0;
}



int GKFparserImpl::endElement(const char* /*name*/)
{
    switch(state)
    {
    case state_gama_xml: state = state_stop; break;
    case state_network: state = state_gama_xml; break;
    case state_description:
        m_lnet.description = m_description;
        state = state_network;
        break;
    case state_parameters: state = state_network; break;
    case state_point_obs: state = state_network; break;
    case state_point: state = state_point_obs; break;


    case state_obs_direction: state = state_obs; break;
    case state_obs_distance: state = state_obs; break;
    case state_obs_angle: state = state_obs; break;
    case state_obs_sdistance: state = state_obs; break;
    case state_obs_zangle: state = state_obs; break;
    case state_obs_azimuth: state = state_obs; break;
    case state_obs_cov: state = state_obs_after_cov; break;
    case state_obs_after_cov:
        state = state_point_obs;
        finish_obs();
        break;
    case state_obs:
        state = state_point_obs;
        finish_obs();
        break;


    case state_hdiffs_dh: state = state_hdiffs; break;
    case state_hdiffs_cov: state = state_hdiffs_after_cov; break;
    case state_hdiffs_after_cov:
        state = state_point_obs;
        finish_hdiffs();
        break;
    case state_hdiffs:
        state = state_point_obs;
        finish_hdiffs();
        break;


    case state_coords_point: state = state_coords; break;
    case state_coords_cov: state = state_coords_after_cov; break;
    case state_coords_after_cov:
        state = state_point_obs;
        finish_coords();
        break;


    case state_vectors_vec: state = state_vectors; break;
    case state_vectors_cov: state = state_vectors_after_cov; break;
    case state_vectors_after_cov:
        state = state_point_obs;
        finish_vectors();
        break;


    case state_stop: state = state_error; break;
    default: state = state_error; break;
    }

    return 0;  // return value is never used in GKFparser
}



GKFparserImpl::gkf_tag GKFparserImpl::tag(const char* c)
{
    switch(*c)
    {
    case 'a':
        if(!strcmp(c, "angle")) return tag_angle;
        if(!strcmp(c, "azimuth")) return tag_azimuth;
        break;
    case 'c':
        if(!strcmp(c, "coordinates")) return tag_coordinates;
        if(!strcmp(c, "cov-mat")) return tag_cov_mat;
        break;
    case 'd':
        if(!strcmp(c, "description")) return tag_description;
        if(!strcmp(c, "dh")) return tag_dh;
        if(!strcmp(c, "direction")) return tag_direction;
        if(!strcmp(c, "distance")) return tag_distance;
        break;
    case 'g':
        if(!strcmp(c, "gama-local")) return tag_gama_xml;
        break;
    case 'h':
        if(!strcmp(c, "height-differences")) return tag_height_differences;
        break;
    case 'n':
        if(!strcmp(c, "network")) return tag_network;
        break;
    case 'o':
        if(!strcmp(c, "obs")) return tag_obs;
        break;
    case 'p':
        if(!strcmp(c, "parameters")) return tag_parameters;
        if(!strcmp(c, "point")) return tag_point;
        if(!strcmp(c, "points-observations")) return tag_points_observations;
        break;
    case 's':
        if(!strcmp(c, "s-distance")) return tag_s_distance;
        break;
    case 'v':
        if(!strcmp(c, "vec")) return tag_vec;
        if(!strcmp(c, "vectors")) return tag_vectors;
        break;
    case 'z':
        if(!strcmp(c, "z-angle")) return tag_z_angle;
        break;
    }

    return tag_unknown;
}



GKFparserImpl::GKFparserImpl(GNU_gama::local::LocalNetwork& locnet) : m_lnet(locnet), m_SB(m_lnet.PD), m_OD(m_lnet.OD)
{
    m_lnet.apriori_m_0(10);
    m_lnet.conf_pr(0.95);
    m_lnet.tol_abs(1000);
    m_lnet.set_m_0_aposteriori();

    state = state_start;
    standpoint = 0;
    m_idim = 0;
    coordinates = 0;
    vectors = 0;

    // throw exception if a covariance matrix is not positive-definite
    check_cov_mat = true;
}



GKFparserImpl::~GKFparserImpl()
{
}



int GKFparserImpl::process_gama_xml(const char** atts)
{
    string nam, val;
    state = state_gama_xml;

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);

        if(nam == "xmlns")
        {
            if(val != XSD_GAMA_LOCAL) { return error("bad namespace xmlns=\"" + val + "\""); }
        }
        else if(nam == "version")
        {
            // ###### ignored for backward compatibility ######
        }
        else
        {
            return error(T_GKF_undefined_attribute_of_gama_xml + nam + " = " + val);
        }
    }

    return 0;
}



int GKFparserImpl::process_network(const char** atts)
{
    string nam, val;
    state = state_network;

    m_lnet.clear_nullable_data();

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);

        if(nam == "axes-xy")
        {
            LocalCoordinateSystem::CS& lcs = m_SB.local_coordinate_system;

            if(val == "ne")
                lcs = LocalCoordinateSystem::CS::NE;
            else if(val == "sw")
                lcs = LocalCoordinateSystem::CS::SW;
            else if(val == "es")
                lcs = LocalCoordinateSystem::CS::ES;
            else if(val == "wn")
                lcs = LocalCoordinateSystem::CS::WN;
            else if(val == "en")
                lcs = LocalCoordinateSystem::CS::EN;
            else if(val == "nw")
                lcs = LocalCoordinateSystem::CS::NW;
            else if(val == "se")
                lcs = LocalCoordinateSystem::CS::SE;
            else if(val == "ws")
                lcs = LocalCoordinateSystem::CS::WS;
            else
                return error(T_GKF_undefined_value_of_attribute + nam + " = " + val);
        }
        else if(nam == "angles")
        {
            if(val == "right-handed")
                m_SB.setAngularObservations_Righthanded();
            else if(val == "left-handed")
                m_SB.setAngularObservations_Lefthanded();
            else
                return error(T_GKF_undefined_value_of_attribute + nam + " = " + val);
        }
        else if(nam == "epoch")
        {
            double epoch;
            if(!toDouble(val, epoch))
                return error(T_GKF_error_on_reading_of_epoch + nam + " = " + val);
            m_lnet.set_epoch(epoch);
        }
        else
        {
            return error(T_GKF_undefined_attribute_of_gama_xml + nam + " = " + val);
        }
    }

    return 0;
}



int GKFparserImpl::process_parameters(const char** atts)
{
    string jmeno, hodnota;
    double dhod;
    state = state_parameters;

    while(*atts)
    {
        jmeno = string(*atts++);
        hodnota = string(*atts++);

        if(jmeno == "sigma-apr")
        {
            if(!toDouble(hodnota, dhod)) return error(T_GKF_error_on_reading_of_standard_deviation);
            if(dhod <= 0) return error(T_GKF_error_on_reading_of_standard_deviation);

            m_lnet.apriori_m_0(dhod);
        }
        else if(jmeno == "conf-pr")
        {
            if(!toDouble(hodnota, dhod))
                return error(T_GKF_error_on_reading_of_confidence_probability);
            if(dhod <= 0) return error(T_GKF_error_on_reading_of_confidence_probability);

            m_lnet.conf_pr(dhod);
        }
        else if(jmeno == "tol-abs")
        {
            if(!toDouble(hodnota, dhod))
                return error(T_GKF_error_on_reading_of_absolute_terms_tolerance);
            if(dhod <= 0) return error(T_GKF_error_on_reading_of_absolute_terms_tolerance);

            m_lnet.tol_abs(dhod);
        }
        else if(jmeno == "sigma-act")
        {
            if(hodnota == "aposteriori")
                m_lnet.set_m_0_aposteriori();
            else if(hodnota == "apriori")
                m_lnet.set_m_0_apriori();
            else
                return error(T_GKF_wrong_type_of_standard_deviation);
        }
        else if(jmeno == "angles")
        {
            if(hodnota == "400")
                m_lnet.set_gons();
            else if(hodnota == "360")
                m_lnet.set_degrees();
            else
                error(T_GKF_bad_network_configuration_unknown_parameter + jmeno + " = " + hodnota);
        }
        else if(jmeno == "algorithm")
        {
            if(hodnota == "gso")
                m_lnet.set_algorithm(GNU_gama::local::LocalNetwork::Algorithm::gso);
            else if(hodnota == "svd")
                m_lnet.set_algorithm(GNU_gama::local::LocalNetwork::Algorithm::svd);
            else if(hodnota == "cholesky")
                m_lnet.set_algorithm(GNU_gama::local::LocalNetwork::Algorithm::cholesky);
            else if(hodnota == "envelope")
                m_lnet.set_algorithm(GNU_gama::local::LocalNetwork::Algorithm::envelope);
        }
        else if(jmeno == "cov-band")
        {
            int ival;
            if(!toInteger(hodnota, ival))
                return error(T_GKF_undefined_value_of_attribute + jmeno + " = " + hodnota);

            m_lnet.set_adj_covband(ival);
        }
        else if(jmeno == "latitude")
        {
            double dm;
            if(!GNU_gama::deg2gon(hodnota, dm))
            {
                if(!toDouble(hodnota, dm))
                    return error(T_GKF_undefined_value_of_attribute + jmeno + " = " + hodnota);
            }
            m_lnet.set_latitude(dm * GNU_gama::PI / 200);
        }
        else if(jmeno == "ellipsoid")
        {
            m_lnet.set_ellipsoid(hodnota);
        }
        else
        {
            return error(T_GKF_bad_network_configuration_unknown_parameter + jmeno + " = " + hodnota);
        }
    }

    return 0;
}



int GKFparserImpl::process_point_obs(const char** atts)
{
    state = state_point_obs;

    // setting values of implicit standard deviations to "undefined";
    // on reaching end tag </points-observations> this state is
    // ignored

    // parameters for implicit stdev for distance
    string nam, val, sds_abc, sds, sdk, sde;

    // implicit standard deviations for direction, angle, zenit-angle, azimuth
    string str_dir = "0", str_ang = "0", str_zen = "0", str_azi = "0";


    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);

        if(nam == "distance-stdev")
        {
            sds_abc = val;
            string::const_iterator i = val.begin();
            while(i != val.end() && isspace(*i)) ++i;
            while(i != val.end() && !isspace(*i))
            {
                sds += *i;
                ++i;
            }
            while(i != val.end() && isspace(*i)) ++i;
            while(i != val.end() && !isspace(*i))
            {
                sdk += *i;
                ++i;
            }
            while(i != val.end() && isspace(*i)) ++i;
            while(i != val.end() && !isspace(*i))
            {
                sde += *i;
                ++i;
            }
            while(i != val.end() && isspace(*i)) ++i;

            if(i != val.end()) return error(T_GKF_bad_attribute_distance_dev + sds_abc);
        }
        else if(nam == "direction-stdev")
            str_dir = val;
        else if(nam == "angle-stdev")
            str_ang = val;
        else if(nam == "zenith-angle-stdev")
            str_zen = val;
        else if(nam == "azimuth-stdev")
            str_azi = val;
        else
            return error(T_GKF_undefined_attribute_of_points_observations + nam + " = " + val);
    }

    if(sds == "") sds = "0";
    if(sdk == "") sdk = "0";
    if(sde == "") sde = "1";
    if(!toDouble(sds, distance_stdev_) || !toDouble(sdk, distance_stdev_km_)
       || !toDouble(sde, distance_stdev_exp_))
        return error(T_GKF_bad_attribute_distance_dev + sds_abc);
    if(!toDouble(str_dir, direction_stdev_))
        return error(T_GKF_bad_attribute_direction_dev + str_dir);
    if(!toDouble(str_ang, angle_stdev_)) return error(T_GKF_bad_attribute_angle_dev + str_ang);
    if(!toDouble(str_zen, zenith_stdev_)) return error(T_GKF_bad_attribute_angle_dev + str_zen);
    if(!toDouble(str_azi, azimuth_stdev_)) return error(T_GKF_bad_attribute_angle_dev + str_azi);

    return 0;
}



int GKFparserImpl::process_point(const char** atts)
{
    string nam, val, sy, sx, sv, sf, sa, st, sh;
    m_pp_xydef = m_pp_zdef = false;
    state = state_point;

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);

        if(nam == "id")
            m_pp_id = val;
        else if(nam == "y")
            sy = val;
        else if(nam == "x")
            sx = val;
        else if(nam == "z")
            sv = val;
        else if(nam == "fix")
            sf = val;
        else if(nam == "adj")
            sa = val;
        else
            return error(T_GKF_undefined_attribute_of_points + nam + " = " + val);
    }

    if(m_pp_id == "") return error(T_GKF_missing_point_ID);

    if(sy != "" && sx == "") return error(T_GKF_coordinate_x_is_not_defined);
    if(sx != "" && sy == "") return error(T_GKF_coordinate_y_is_not_defined);

    if(sx != "")
    {
        double dy, dx;
        if(!toDouble(sx, dx)) return error(T_GKF_bad_coordinate_x + sx);
        if(!toDouble(sy, dy)) return error(T_GKF_bad_coordinate_y + sy);
        m_SB[m_pp_id].set_xy(dx, dy);

        if(m_pp_xydef) return error(T_GKF_multiple_definition_of_xy_in_tag_point);
        m_pp_x = dx;
        m_pp_y = dy;
        m_pp_xydef = true;
    }

    if(sv != "")
    {
        double dz;
        if(!toDouble(sv, dz)) return error(T_GKF_bad_height + sv);
        m_SB[m_pp_id].set_z(dz);

        if(m_pp_zdef) return error(T_GKF_multiple_definition_of_z_in_tag_point);
        m_pp_z = dz;
        m_pp_zdef = true;
    }

    if(sa != "")
    {
        if(sa == "xy")
            m_SB[m_pp_id].set_free_xy();
        else if(sa == "xyz")
            m_SB[m_pp_id].set_free_xy(), m_SB[m_pp_id].set_free_z();
        else if(sa == "z")
            m_SB[m_pp_id].set_free_z();
        else if(sa == "XY")
            m_SB[m_pp_id].set_constrained_xy();
        else if(sa == "XYZ")
            m_SB[m_pp_id].set_constrained_xy(), m_SB[m_pp_id].set_constrained_z();
        else if(sa == "XYz")
            m_SB[m_pp_id].set_constrained_xy(), m_SB[m_pp_id].set_free_z();
        else if(sa == "xyZ")
            m_SB[m_pp_id].set_free_xy(), m_SB[m_pp_id].set_constrained_z();
        else if(sa == "Z")
            m_SB[m_pp_id].set_constrained_z();
        else
            return error(T_GKF_undefined_point_type + sa);
    }

    if(sf != "")
    {
        if(sf == "xy")
            m_SB[m_pp_id].set_fixed_xy();
        else if(sf == "xyz")
            m_SB[m_pp_id].set_fixed_xy(), m_SB[m_pp_id].set_fixed_z();
        else if(sf == "z")
            m_SB[m_pp_id].set_fixed_z();
        else if(sf == "XY")
            m_SB[m_pp_id].set_fixed_xy();
        else if(sf == "XYZ")
            m_SB[m_pp_id].set_fixed_xy(), m_SB[m_pp_id].set_fixed_z();
        else if(sf == "XYz")
            m_SB[m_pp_id].set_fixed_xy(), m_SB[m_pp_id].set_fixed_z();
        else if(sf == "xyZ")
            m_SB[m_pp_id].set_fixed_xy(), m_SB[m_pp_id].set_fixed_z();
        else if(sf == "Z")
            m_SB[m_pp_id].set_fixed_z();
        else
            return error(T_GKF_undefined_point_type + sf);
    }

    return 0;
}



int GKFparserImpl::process_distance(const char** atts)
{
    string nam, val, ss = standpoint_id, sc, sm, sv, hf, ht, ex;
    state = state_obs_distance;

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);
        if(nam == "from")
            ss = val;
        else if(nam == "to")
            sc = val;
        else if(nam == "val")
            sm = val;
        else if(nam == "stdev")
            sv = val;
        else if(nam == "from_dh")
            hf = val;
        else if(nam == "to_dh")
            ht = val;
        else if(nam == "extern")
            ex = val;
        else
            return error(T_GKF_undefined_attribute_of_distance + nam + " = " + val);
    }

    if(ss == "") return error(T_GKF_missing_standpoint_id);
    if(sc == "") return error(T_GKF_missing_forepoint_id);
    if(sm == "") return error(T_GKF_missing_observed_value);

    double dm;
    if(!toDouble(sm, dm)) return error(T_GKF_bad_distance + sm);
    double dv = implicit_stdev_distance(dm);
    if(sv != "")
        if(!toDouble(sv, dv)) return error(T_GKF_illegal_standard_deviation);
    double df = obs_from_dh;
    if(hf != "")
        if(!toDouble(hf, df)) return error(T_GKF_bad_instrument_reflector_height + hf);
    double dt = 0;
    if(ht != "")
        if(!toDouble(ht, dt)) return error(T_GKF_bad_instrument_reflector_height + ht);

    try
    {
        if(standpoint == 0)
        {
            standpoint = new StandPoint(&m_OD);
            m_OD.clusters.push_back(standpoint);
        }
        Distance* d = new Distance(ss, sc, dm);
        d->set_extern(ex);
        d->set_from_dh(df);
        d->set_to_dh(dt);
        standpoint->observation_list.push_back(d);
        m_sigma.push_back(DB_pair(dv, false));
    }
    catch(const /*GNU_gama::local::*/ Exception& e)
    {
        return error(e.what());
    }

    return 0;
}



int GKFparserImpl::process_angle(const char** atts)
{
    bool degrees = false;
    string nam, val, ss = standpoint_id, sl, sp, sm, sv, hf, ht, h2, ex;
    state = state_obs_angle;

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);

        if(nam == "from")
            ss = val;
        else if(nam == "bs")
            sl = val;  // backsight station
        else if(nam == "fs")
            sp = val;  // foresight station
        else if(nam == "to")
            sl = val;  // <== undocumented feature for
        else if(nam == "rs")
            sp = val;  // <== backward compatibility
        else if(nam == "val")
            sm = val;
        else if(nam == "stdev")
            sv = val;
        else if(nam == "from_dh")
            hf = val;
        else if(nam == "bs_dh")
            ht = val;
        else if(nam == "fs_dh")
            h2 = val;
        else if(nam == "extern")
            ex = val;
        else
            return error(T_GKF_undefined_attribute_of_angle + nam + " = " + val);
    }

    if(ss == "") return error(T_GKF_missing_standpoint_id);
    if(sl == "") return error(T_GKF_missing_left_forepoint_id);
    if(sp == "") return error(T_GKF_missing_right_forepoint_id);
    if(sm == "") return error(T_GKF_missing_observed_value);

    double dm;
    if(GNU_gama::deg2gon(sm, dm))
        degrees = true;
    else if(!toDouble(sm, dm))
        return error(T_GKF_bad_angle + sm);

    double dv = implicit_stdev_angle();
    if(sv != "")
        if(!toDouble(sv, dv)) return error(T_GKF_illegal_standard_deviation);
    double df = obs_from_dh;
    if(hf != "")
        if(!toDouble(hf, df)) return error(T_GKF_bad_instrument_reflector_height + hf);
    double dt = 0;
    if(ht != "")
        if(!toDouble(ht, dt)) return error(T_GKF_bad_instrument_reflector_height + ht);
    double d2 = 0;
    if(h2 != "")
        if(!toDouble(h2, d2)) return error(T_GKF_bad_instrument_reflector_height + h2);

    try
    {
        if(standpoint == 0)
        {
            standpoint = new StandPoint(&m_OD);
            m_OD.clusters.push_back(standpoint);
        }
        Angle* d = new Angle(ss, sl, sp, dm * GNU_gama::GON_TO_RAD);
        d->set_extern(ex);
        d->set_from_dh(df);
        d->set_to_dh(dt);
        standpoint->observation_list.push_back(d);
        m_sigma.push_back(DB_pair(dv, degrees));
    }
    catch(const /*GNU_gama::local::*/ Exception& e)
    {
        error(e.what());
    }

    return 0;
}



int GKFparserImpl::process_sdistance(const char** atts)
{
    string nam, val, ss = standpoint_id, sc, sm, sv, hf, ht, ex;
    state = state_obs_sdistance;

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);
        if(nam == "from")
            ss = val;
        else if(nam == "to")
            sc = val;
        else if(nam == "val")
            sm = val;
        else if(nam == "stdev")
            sv = val;
        else if(nam == "from_dh")
            hf = val;
        else if(nam == "to_dh")
            ht = val;
        else if(nam == "extern")
            ex = val;
        else
            return error(T_GKF_undefined_attribute_of_slopedist + nam + " = " + val);
    }

    if(ss == "") return error(T_GKF_missing_standpoint_id);
    if(sc == "") return error(T_GKF_missing_forepoint_id);
    if(sm == "") return error(T_GKF_missing_observed_value);

    double dm;
    if(!toDouble(sm, dm)) return error(T_GKF_bad_distance + sm);
    double dv = implicit_stdev_distance(dm);
    if(sv != "")
        if(!toDouble(sv, dv)) return error(T_GKF_illegal_standard_deviation);
    double df = obs_from_dh;
    if(hf != "")
        if(!toDouble(hf, df)) return error(T_GKF_bad_instrument_reflector_height + hf);
    double dt = 0;
    if(ht != "")
        if(!toDouble(ht, dt)) return error(T_GKF_bad_instrument_reflector_height + ht);

    try
    {
        if(standpoint == 0)
        {
            standpoint = new StandPoint(&m_OD);
            m_OD.clusters.push_back(standpoint);
        }
        S_Distance* d = new S_Distance(ss, sc, dm);
        d->set_extern(ex);
        d->set_from_dh(df);
        d->set_to_dh(dt);
        standpoint->observation_list.push_back(d);
        m_sigma.push_back(DB_pair(dv, false));
    }
    catch(const /*GNU_gama::local::*/ Exception& e)
    {
        return error(e.what());
    }

    return 0;
}



int GKFparserImpl::process_zangle(const char** atts)
{
    bool degrees = false;
    string nam, val, ss = standpoint_id, sc, sm, sv, hf, ht, ex;
    state = state_obs_zangle;

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);
        if(nam == "from")
            ss = val;
        else if(nam == "to")
            sc = val;
        else if(nam == "val")
            sm = val;
        else if(nam == "stdev")
            sv = val;
        else if(nam == "from_dh")
            hf = val;
        else if(nam == "to_dh")
            ht = val;
        else if(nam == "extern")
            ex = val;
        else
            return error(T_GKF_undefined_attribute_of_zangle + nam + " = " + val);
    }

    if(ss == "") return error(T_GKF_missing_standpoint_id);
    if(sc == "") return error(T_GKF_missing_forepoint_id);
    if(sm == "") return error(T_GKF_missing_observed_value);

    double dm;
    if(GNU_gama::deg2gon(sm, dm))
        degrees = true;
    else if(!toDouble(sm, dm))
        return error(T_GKF_bad_zangle + sm);

    double dv = implicit_stdev_zangle();
    if(sv != "")
        if(!toDouble(sv, dv)) return error(T_GKF_illegal_standard_deviation);
    double df = obs_from_dh;
    if(hf != "")
        if(!toDouble(hf, df)) return error(T_GKF_bad_instrument_reflector_height + hf);
    double dt = 0;
    if(ht != "")
        if(!toDouble(ht, dt)) return error(T_GKF_bad_instrument_reflector_height + ht);

    try
    {
        if(standpoint == 0)
        {
            standpoint = new StandPoint(&m_OD);
            m_OD.clusters.push_back(standpoint);
        }
        Z_Angle* d = new Z_Angle(ss, sc, dm * GNU_gama::GON_TO_RAD);
        d->set_extern(ex);
        d->set_from_dh(df);
        d->set_to_dh(dt);
        standpoint->observation_list.push_back(d);
        m_sigma.push_back(DB_pair(dv, degrees));
    }
    catch(const /*GNU_gama::local::*/ Exception& e)
    {
        return error(e.what());
    }

    return 0;
}



int GKFparserImpl::process_obs(const char** atts)
{
    string nam, val, ss, sz, sh;
    obs_from_dh = 0;  // implicit instrument height for <obs />
    state = state_obs;

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);

        if(nam == "from")
            ss = val;
        else if(nam == "orientation")
            sz = val;
        else if(nam == "from_dh")
            sh = val;
        else
            return error(T_GKF_undefined_attribute_of_obs + nam + " = " + val);
    }

    m_idim = 0;
    standpoint_id = ss;
    standpoint = new StandPoint(&m_OD);
    standpoint->station = standpoint_id;
    if(sz != "")
    {
        double dz;
        if(!toDouble(sz, dz)) return error(T_GKF_bad_orientation_angle + sz);
        standpoint->set_orientation(dz);
    }
    if(sh != "")
    {
        if(!toDouble(sh, obs_from_dh)) return error(T_GKF_bad_instrument_reflector_height + sh);
    }
    m_OD.clusters.push_back(standpoint);

    return 0;
}



int GKFparserImpl::finish_obs()
{
    standpoint->update();  // bind observations to the cluster
    if(m_idim) { finish_cov(standpoint->covariance_matrix); }
    else
    {
        const int N = m_sigma.size();
        standpoint->covariance_matrix.reset(N, 0);
        CovMat::iterator c = standpoint->covariance_matrix.begin();
        std::vector<DB_pair>::iterator s = m_sigma.begin();

        for(int i = 1; i <= N; ++i, ++c, ++s) *c = (*s).first * (*s).first;
    }

    if(check_cov_mat)
    {
        try
        {
            // scaling of rows/columns corresponding to covariances
            // given in sexagesimal seconds
            std::vector<DB_pair>::iterator s = m_sigma.begin();
            for(int i = 1; i <= size_to<int>(m_sigma.size()); ++i, ++s)
            {
                if((*s).second) standpoint->scaleCov(i, 1.0 / 0.324);
            }

            CovMat tmp = standpoint->covariance_matrix;
            tmp.cholDec();
        }
        catch(...)
        {
            return error(T_GKF_covariance_matrix_is_not_positive_definite);
        }
    }

    standpoint = 0;
    standpoint_id = "";
    m_sigma.erase(m_sigma.begin(), m_sigma.end());
    return 0;
}



int GKFparserImpl::process_direction(const char** atts)
{
    bool degrees = false;
    string nam, val, sc, sm, ss, hf, ht, ex;
    state = state_obs_direction;

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);

        if(nam == "to")
            sc = val;
        else if(nam == "val")
            sm = val;
        else if(nam == "stdev")
            ss = val;
        else if(nam == "from_dh")
            hf = val;
        else if(nam == "to_dh")
            ht = val;
        else if(nam == "extern")
            ex = val;
        else
            return error(T_GKF_undefined_attribute_of_direction + nam + " = " + val);
    }

    if(standpoint_id == "") return error(T_GKF_missing_standpoint_id);
    if(sc == "") return error(T_GKF_missing_forepoint_id);
    if(sm == "") return error(T_GKF_missing_observed_value);

    double dm;
    if(GNU_gama::deg2gon(sm, dm))
        degrees = true;
    else if(!toDouble(sm, dm))
        return error(T_GKF_bad_direction + sm);

    double ds = implicit_stdev_direction();
    if(ss != "")
        if(!toDouble(ss, ds)) return error(T_GKF_illegal_standard_deviation);
    double df = obs_from_dh;
    if(hf != "")
        if(!toDouble(hf, df)) return error(T_GKF_bad_instrument_reflector_height + hf);
    double dt = 0;
    if(ht != "")
        if(!toDouble(ht, dt)) return error(T_GKF_bad_instrument_reflector_height + ht);

    try
    {
        Direction* d = new Direction(standpoint_id, sc, dm * GNU_gama::GON_TO_RAD);
        d->set_extern(ex);
        d->set_from_dh(df);
        d->set_to_dh(dt);
        standpoint->observation_list.push_back(d);
        m_sigma.push_back(DB_pair(ds, degrees));
    }
    catch(const /*GNU_gama::local::*/ Exception& e)
    {
        error(e.what());
    }

    return 0;
}



int GKFparserImpl::process_azimuth(const char** atts)
{
    bool degrees = false;
    string nam, val, ss = standpoint_id, sc, sm, sv, hf, ht, ex;
    state = state_obs_azimuth;

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);

        if(nam == "from")
            ss = val;
        else if(nam == "to")
            sc = val;
        else if(nam == "val")
            sm = val;
        else if(nam == "stdev")
            sv = val;
        else if(nam == "from_dh")
            hf = val;
        else if(nam == "to_dh")
            ht = val;
        else if(nam == "extern")
            ex = val;
        else
            return error(T_GKF_undefined_attribute_of_azimuth + nam + " = " + val);
    }

    if(ss == "") return error(T_GKF_missing_standpoint_id);
    if(sc == "") return error(T_GKF_missing_forepoint_id);
    if(sm == "") return error(T_GKF_missing_observed_value);

    double dm;
    if(GNU_gama::deg2gon(sm, dm))
        degrees = true;
    else if(!toDouble(sm, dm))
        return error(T_GKF_bad_azimuth + sm);

    double ds = implicit_stdev_azimuth();
    if(sv != "")
        if(!toDouble(sv, ds)) return error(T_GKF_illegal_standard_deviation);
    double df = obs_from_dh;
    if(hf != "")
        if(!toDouble(hf, df)) return error(T_GKF_bad_instrument_reflector_height + hf);
    double dt = 0;
    if(ht != "")
        if(!toDouble(ht, dt)) return error(T_GKF_bad_instrument_reflector_height + ht);

    try
    {
        Azimuth* d = new Azimuth(ss, sc, dm * GNU_gama::GON_TO_RAD);
        d->set_extern(ex);
        d->set_from_dh(df);
        d->set_to_dh(dt);
        standpoint->observation_list.push_back(d);
        m_sigma.push_back(DB_pair(ds, degrees));
    }
    catch(const /*GNU_gama::local::*/ Exception& e)
    {
        error(e.what());
    }

    return 0;
}


int GKFparserImpl::process_cov(const char** atts)
{
    string nam, val, sdim, sband;

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);

        if(nam == "dim")
            sdim = val;
        else if(nam == "band")
            sband = val;
        else
            return error(T_GKF_undefined_attribute_of_cov_mat + nam + " = " + val);
    }

    if(sdim == "") return error(T_GKF_cov_mat_missing_dim);
    if(sband == "") return error(T_GKF_cov_mat_missing_band_width);

    if(!toIndex(sdim, m_idim)) return error(T_GKF_cov_mat_bad_dim + nam + " = " + val);
    if(!toIndex(sband, m_iband)) return error(T_GKF_cov_mat_bad_band_width + nam + " = " + val);

    if(m_idim < 1) return error(T_GKF_cov_mat_bad_dim + nam + " = " + val);
    if(isNegative(m_iband) || m_iband >= m_idim)
        return error(T_GKF_cov_mat_bad_band_width + nam + " = " + val);

    return 0;
}


int GKFparserImpl::finish_cov(CovMat& cov_mat)
{
    cov_mat.reset(m_idim, m_iband);
    int elements = m_idim * (m_iband + 1) - m_iband * (m_iband + 1) / 2;
    string::const_iterator i = m_cov_mat_data.begin();
    int row = 1;
    int col = row;

    while(i != m_cov_mat_data.end())
    {
        while(i != m_cov_mat_data.end() && isspace(*i)) ++i;
        string w;
        while(i != m_cov_mat_data.end() && !isspace(*i))
        {
            w += *i;
            ++i;
        }
        if(w.size())
        {
            if(elements == 0) return error(T_GKF_cov_mat_bad_dim_too_many_elements);
            double d;
            if(!toDouble(w, d)) return error(T_GKF_cov_mat_bad_element);
            cov_mat(row, col) = d;
            elements--;
            col++;
            if(col > row + m_iband || col > m_idim) col = ++row;
        }
    }

    if(elements) return error(T_GKF_cov_mat_bad_dim_not_enough_elements);

    m_idim = 0;
    m_cov_mat_data = "";
    return 0;
}


int GKFparserImpl::process_coords(const char** atts)
{
    state = state_coords;
    string ext;

    if(*atts)
    {
        string nam, val;
        nam = string(*atts++);
        val = string(*atts++);
        if(nam == "extern") { ext = val; }
        else
        {
            return error(T_GKF_undefined_attribute_of_coordinates + nam + " = " + val);
        }
    }

    coordinates = new Coordinates(&m_OD);
    if(!ext.empty()) coordinates->set_extern(ext);

    m_OD.clusters.push_back(coordinates);

    return 0;
}


int GKFparserImpl::finish_coords()
{
    if(!m_idim) return error(T_GKF_coordinates_without_covariance_matrix);
    if(m_idim != size_to<int>(coordinates->observation_list.size()))
        return error("T_GKF_cov_dim_differs_from_number_of_coordinates");

    coordinates->update();
    finish_cov(coordinates->covariance_matrix);

    if(check_cov_mat)
    {
        try
        {
            CovMat tmp = coordinates->covariance_matrix;
            std::cout << tmp ;
            tmp.cholDec();
        }
        catch(...)
        {
            return error(T_GKF_covariance_matrix_is_not_positive_definite);
        }
    }

    coordinates = 0;
    return 0;
}


int GKFparserImpl::process_coords_point(const char** atts)
{
    process_point(atts);
    state = state_coords_point;  // we must reset state here !!!

    if(!m_pp_xydef && !m_pp_zdef)
        return error(T_GKF_point_must_define_xy_andor_z_inside_tag_coordinates);

    if(m_pp_xydef)
    {
        coordinates->observation_list.push_back(new X(m_pp_id, m_pp_x));
        coordinates->observation_list.push_back(new Y(m_pp_id, m_pp_y));
    }
    if(m_pp_zdef) { coordinates->observation_list.push_back(new Z(m_pp_id, m_pp_z)); }

    return 0;
}


int GKFparserImpl::process_hdiffs(const char** atts)
{
    state = state_hdiffs;

    if(*atts)
    {
        string nam, val;
        nam = string(*atts++);
        val = string(*atts++);
        return error(T_GKF_undefined_attribute_of_height_differences + nam + " = " + val);
    }

    heightdifferences = new HeightDifferences(&m_OD);
    m_OD.clusters.push_back(heightdifferences);

    return 0;
}


int GKFparserImpl::finish_hdiffs()
{
    heightdifferences->update();  // bind observations to the cluster
    if(m_idim) { finish_cov(heightdifferences->covariance_matrix); }
    else
    {
        const int N = m_sigma.size();
        heightdifferences->covariance_matrix.reset(N, 0);
        CovMat::iterator c = heightdifferences->covariance_matrix.begin();
        std::vector<DB_pair>::iterator s = m_sigma.begin();

        for(int i = 1; i <= N; ++i, ++c, ++s) *c = (*s).first * (*s).first;
    }

    if(check_cov_mat)
    {
        try
        {
            CovMat tmp = heightdifferences->covariance_matrix;
            tmp.cholDec();
        }
        catch(...)
        {
            return error(T_GKF_covariance_matrix_is_not_positive_definite);
        }
    }

    heightdifferences = 0;
    m_sigma.erase(m_sigma.begin(), m_sigma.end());

    return 0;
}

int GKFparserImpl::process_dh(const char** atts)
{
    string nam, val, sfrom, sto, sval, sstdev, sdist, ex;
    state = state_hdiffs_dh;

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);

        if(nam == "from")
            sfrom = val;
        else if(nam == "to")
            sto = val;
        else if(nam == "val")
            sval = val;
        else if(nam == "stdev")
            sstdev = val;
        else if(nam == "dist")
            sdist = val;
        else if(nam == "extern")
            ex = val;
        else
            return error(T_GKF_undefined_attribute_of_height_differences + nam + " = " + val);
    }

    if(sfrom == "") return error(T_GKF_missing_from_ID);
    if(sto == "") return error(T_GKF_missing_to_ID);
    if(sval == "") return error(T_GKF_missing_observed_value);

    double dm;
    if(!toDouble(sval, dm)) return error(T_GKF_bad_height_diff + sval);
    double dd = 0;
    if(sdist != "")
        if(!toDouble(sdist, dd) || dd < 0) return error(T_GKF_bad_distance + sdist);
    double ds = m_lnet.apriori_m_0() * sqrt(dd);
    if(sstdev != "")
        if(!toDouble(sstdev, ds)) return error(T_GKF_illegal_standard_deviation);

    try
    {
        H_Diff* hd = new H_Diff(sfrom, sto, dm, dd);
        hd->set_extern(ex);
        heightdifferences->observation_list.push_back(hd);
        m_sigma.push_back(DB_pair(ds, false));
    }
    catch(const /*GNU_gama::local::*/ Exception& e)
    {
        error(e.what());
    }

    return 0;
}


int GKFparserImpl::process_vectors(const char** atts)
{
    state = state_vectors;

    if(*atts)
    {
        string nam, val;
        nam = string(*atts++);
        val = string(*atts++);
        return error(T_GKF_undefined_attribute_of_vectors + nam + " = " + val);
    }

    vectors = new Vectors(&m_OD);
    m_OD.clusters.push_back(vectors);

    return 0;
}


int GKFparserImpl::finish_vectors()
{
    if(!m_idim) return error(T_GKF_vectors_without_covariance_matrix);
    if(m_idim != size_to<int>(vectors->observation_list.size()))
        return error("T_GKF_cov_dim_differs_from_number_of_vectors");

    vectors->update();  // bind observations to the cluster
    finish_cov(vectors->covariance_matrix);

    if(check_cov_mat)
    {
        try
        {
            CovMat tmp = vectors->covariance_matrix;
            tmp.cholDec();
        }
        catch(...)
        {
            return error(T_GKF_covariance_matrix_is_not_positive_definite);
        }
    }

    vectors = 0;
    return 0;
}


int GKFparserImpl::process_vec(const char** atts)
{
    string nam, val, sfrom, sto, sdx, sdy, sdz, hf, ht, ex;
    state = state_vectors_vec;

    while(*atts)
    {
        nam = string(*atts++);
        val = string(*atts++);

        if(nam == "from")
            sfrom = val;
        else if(nam == "to")
            sto = val;
        else if(nam == "dx")
            sdx = val;
        else if(nam == "dy")
            sdy = val;
        else if(nam == "dz")
            sdz = val;
        else if(nam == "from_dh")
            hf = val;
        else if(nam == "to_dh")
            ht = val;
        else if(nam == "extern")
            ex = val;
        else
            return error(T_GKF_undefined_attribute_of_height_differences + nam + " = " + val);
    }

    if(sfrom == "") return error(T_GKF_missing_from_ID);
    if(sto == "") return error(T_GKF_missing_to_ID);
    if(sdx == "" || sdy == "" || sdz == "") return error(T_GKF_bad_vector_data);

    double dx, dy, dz;
    if(!toDouble(sdx, dx) || !toDouble(sdy, dy) || !toDouble(sdz, dz))
        return error(T_GKF_bad_vector_data);
    double df = 0;
    if(hf != "")
        if(!toDouble(hf, df)) return error(T_GKF_bad_instrument_reflector_height + hf);
    double dt = 0;
    if(ht != "")
        if(!toDouble(ht, dt)) return error(T_GKF_bad_instrument_reflector_height + ht);

    try
    {
        Xdiff* xdiff = new Xdiff(sfrom, sto, dx);
        Ydiff* ydiff = new Ydiff(sfrom, sto, dy);
        Zdiff* zdiff = new Zdiff(sfrom, sto, dz);

        xdiff->set_from_dh(df);
        xdiff->set_to_dh(dt);
        ydiff->set_from_dh(df);
        ydiff->set_to_dh(dt);
        zdiff->set_from_dh(df);
        zdiff->set_to_dh(dt);

        xdiff->set_extern(ex);
        ydiff->set_extern(ex);
        zdiff->set_extern(ex);

        vectors->observation_list.push_back(xdiff);
        vectors->observation_list.push_back(ydiff);
        vectors->observation_list.push_back(zdiff);
    }
    catch(const /*GNU_gama::local::*/ Exception& e)
    {
        error(e.what());
    }

    return 0;
}


std::istream& operator>>(std::istream& input, GNU_gama::local::LocalNetwork& network)
{
    // TODO this is not correct. Theoretically we should first parse the stream
    // in a tmp network and if it succeds, then we move it to the one passed.
    // Now if parsing fails, we leave the input network in a non valid state

    GKFparserImpl gkf(network);
        
    std::string line;
    while(std::getline(input, line, input.widen('\n')))
    {
        if(!line.empty())
        {
            gkf.xml_parse(line.c_str(), line.size(), !input ? 1 : 0); 
        }
    } 

    return input;
}

}  // namespace GKFparser
}  // namespace local
}  // namespace GNU_gama
