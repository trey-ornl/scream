#ifndef SCREAM_ATMOSPHERE_NUDGING_HPP
#define SCREAM_ATMOSPHERE_NUDGING_HPP

#include "share/atm_process/atmosphere_process.hpp"
#include "ekat/ekat_parameter_list.hpp"
#include "ekat/util/ekat_lin_interp.hpp"

//#include "share/scream_types.hpp"

#include <string>

namespace scream
{

/*
 * The class responsible to handle the calculation of the subgrid cloud fractions
 *
 * The AD should store exactly ONE instance of this class stored
 * in its list of subcomponents (the AD should make sure of this).
*/

class NUDGING : public AtmosphereProcess
{
public:

  //using gid_type         = AbstractGrid::gid_type;
  //using KT = KokkosTypes<Device>;
  //template <typename S>
  //using view_1d = typename KT::template view_1d<S>;
  //using view_1d_dof     = typename view_1d<gid_type>;
  
  template <typename S>
  using SmallPack = ekat::Pack<S,SCREAM_SMALL_PACK_SIZE>;

  using Spack = SmallPack<Real>;
  using Pack = ekat::Pack<Real,SCREAM_PACK_SIZE>;
  using KT = KokkosTypes<DefaultDevice>;

  template <typename S>
  using view_2d = typename KT::template view_2d<S>;

  template <typename S, int N>
  using view_Nd_host = typename KT::template view_ND<S,N>::HostMirror;

  template <typename S>
  using view_1d_host = view_Nd_host<S,1>;


  // Constructors
  NUDGING (const ekat::Comm& comm, const ekat::ParameterList& params);

  // The type of subcomponent
  AtmosphereProcessType type () const { return AtmosphereProcessType::Physics; }

  // The name of the subcomponent
  std::string name () const { return "Nudging"; }

  // Set the grid
  void set_grids (const std::shared_ptr<const GridsManager> grids_manager);

protected:

  // The three main overrides for the subcomponent
  void initialize_impl (const RunType run_type);
  void run_impl        (const int dt);
  void finalize_impl   ();

  // Computes total number of bytes needed for local variables
  //size_t requested_buffer_size_in_bytes() const;

  // Set local variables using memory provided by
  // the ATMBufferManager
  void init_buffers(const ATMBufferManager &buffer_manager);


  std::shared_ptr<const AbstractGrid>   m_grid;
  // Keep track of field dimensions and the iteration count
  ekat::Comm m_comm;
  int m_num_cols; 
  int m_num_levs;
  //int m_num_src_levs;
  int m_num_src_levs;
  std::string datafile;
  view_2d<Real> T_mid_r_m;
  //FieldLayout scalar3d_layout_mid_;
  // DOF information
  //view_1d_dof m_dofs_gids;
  //int         m_total_global_dofs; // Needed to make sure that remap data matches grid.
  //gid_type    m_min_global_dof;


}; // class NUDGING

} // namespace scream

#endif // SCREAM_ATMOSPHERE_NUDGING_HPP