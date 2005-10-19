/*  
    GNU Gama -- adjustment of geodetic networks
    Copyright (C) 2003  Ales Cepek <cepek@gnu.org>

    This file is part of the GNU Gama C++ library.
    
    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*
 *  $Id: g3_model.h,v 1.40 2005/10/19 16:12:02 cepek Exp $
 */

#ifndef GNU_gama__g3_model_h_gnugamag3modelh___gnu_gama_g3model
#define GNU_gama__g3_model_h_gnugamag3modelh___gnu_gama_g3model

#include <gnu_gama/model.h>
#include <gnu_gama/pointbase.h>
#include <gnu_gama/obsdata.h>
#include <gnu_gama/list.h>
#include <gnu_gama/ellipsoids.h>
#include <gnu_gama/g3/g3_point.h>
#include <gnu_gama/g3/g3_observation.h>
#include <gnu_gama/sparse/smatrix.h>
#include <gnu_gama/sparse/sbdiagonal.h>
#include <gnu_gama/adj/adj.h>
#include <gnu_gama/e3.h>

namespace GNU_gama {  namespace g3 {

    class Model : 
    public GNU_gama::Model<g3::Observation>,
    public Revision     <Distance>,
    public Linearization<Distance>,
    public Revision     <Vector>,
    public Linearization<Vector>,
    public Revision     <XYZ>,
    public Linearization<XYZ>,
    public Revision     <ZenithAngle>,
    public Linearization<ZenithAngle>,
    public Revision     <Azimuth>,
    public Linearization<Azimuth>,
    public Revision     <HeightDiff>,
    public Linearization<HeightDiff>,
    public Revision     <Height>,
    public Linearization<Height>,
    public Revision     <Angle>,
    public Linearization<Angle>
  {
  public:
    
    Model();
    virtual ~Model();    
    
    typedef GNU_gama::ObservationData<g3::Observation>  ObservationData;
    typedef GNU_gama::List<Observation*>                ObservationList; 
    typedef GNU_gama::PointBase<g3::Point>              PointBase;
    typedef GNU_gama::List<Parameter*>                  ParameterList;
    typedef GNU_gama::Adj                               Adj;


    PointBase           *points;    
    GNU_gama::Ellipsoid  ellipsoid;
    
    Point* get_point(const Point::Name&);
    void   write_xml(std::ostream& out) const;
    
    void reset()               { state_ = init_; }
    void reset_parameters()    { if (params_ < state_) state_ = params_; }
    void reset_observations()  { if (obsrvs_ < state_) state_ = obsrvs_; }
    void reset_linearization() { if (linear_ < state_) state_ = linear_; }
    void reset_adjustment()    { if (adjust_ < state_) state_ = adjust_; }
    
    bool check_parameters()    const { return state_ > params_; }
    bool check_observations()  const { return state_ > obsrvs_; }
    bool check_linearization() const { return state_ > linear_; }
    bool check_adjustment()    const { return state_ > adjust_; }
    
    void update_parameters();
    void update_observations();
    void update_linearization();
    void update_adjustment();
        
    bool revision_visit     (Distance*   );
    void linearization_visit(Distance*   );
    bool revision_visit     (Vector*     );
    void linearization_visit(Vector*     );
    bool revision_visit     (XYZ*        );
    void linearization_visit(XYZ*        );
    bool revision_visit     (ZenithAngle*);
    void linearization_visit(ZenithAngle*);
    bool revision_visit     (Azimuth*    );
    void linearization_visit(Azimuth*    );
    bool revision_visit     (HeightDiff* );
    void linearization_visit(HeightDiff* );
    bool revision_visit     (Height*     );
    void linearization_visit(Height*     );
    bool revision_visit     (Angle*      );
    void linearization_visit(Angle*      );

    void write_xml_adjusted(std::ostream&, const Vector*, Index);
    void write_xml_adjusted(std::ostream&, const Height*, Index);

    void set_algorithm(Adj::algorithm a) { adj->set_algorithm(a); }

    void   set_apriori_sd(double s) { apriori_sd = s;          }
    double get_apriori_sd() const   { return apriori_sd;       } 
    void   set_conf_level(double c) { confidence_level = c;    }
    double get_conf_level() const   { return confidence_level; } 

    double standard_deviation() const { return std_deviation; }
    double q_xx(Index i, Index j) { return adj->q_xx(i,j); }

    void write_xml_adjustment_input_data(std::ostream&);
    void write_xml_adjustment_results   (std::ostream&);

    bool angular_units_degrees() const { return !gons_; }
    bool angular_units_gons   () const { return  gons_; }
    void set_angular_units_degrees()   { gons_ = false; }
    void set_angular_units_gons()      { gons_ = true; }
    
    GNU_gama::E_3 vector    (const Point* from, const Point* to) const;
    GNU_gama::E_3 normal    (const Point* p) const;
    GNU_gama::E_3 vertical  (const Point* p) const;
    GNU_gama::E_3 instrument(const Point* p, double dh) const;

  private:   /*-----------------------------------------------------------*/
      
    Model(const Model&);
    Model& operator=(const Model&);
    
    // active observations list (active observations used in the adjustment)
    ObservationList*  active_obs;

    // parameter list
    ParameterList*  par_list;
    void update_index(Parameter&);
    
    // basic revision steps 
    enum State_{init_, params_, obsrvs_, linear_, adjust_, ready_} state_;
    
    void next_state_(int s) { state_ = State_(++s); }
    bool check_init() const { return state_ > init_; }
    void update_init();
        

    // design matrix
    int dm_rows, dm_cols, dm_floats;
    SparseMatrix <>*  A;
    Vec          <>   rhs;
    int               rhs_ind;
    BlockDiagonal<>*  B;
    GNU_gama::AdjInputData*  adj_input_data;

    // adjustment
    Adj*              adj;

    int    redundancy;
    enum { apriori, aposteriori } actual_sd;
    double aposteriori_sd;
    double std_deviation;
    

    // constants
    double apriori_sd;
    double confidence_level;

    bool   gons_;


    void write_xml_adjustment_results_points      (std::ostream&);
    void write_xml_adjustment_results_observations(std::ostream&);

  };
  
}}

#endif
