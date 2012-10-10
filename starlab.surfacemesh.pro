TEMPLATE = subdirs
CONFIG += ordered 

SUBDIRS += surfacemesh

# I/O Plugins
SUBDIRS += surfacemesh_io_off
SUBDIRS += surfacemesh_io_obj

# Rendering Plugins
SUBDIRS += surfacemesh_render_verts
#SUBDIRS += surfacemesh_render_flat
#SUBDIRS += surfacemesh_render_smooth
#SUBDIRS += surfacemesh_render_wireframe
#SUBDIRS += surfacemesh_render_flatwire
#SUBDIRS += surfacemesh_render_transparent

# Filter Plugins
SUBDIRS += surfacemesh_filter_normalize
#SUBDIRS += surfacemesh_decorate_normals
#SUBDIRS += surfacemesh_decorate_selection
#SUBDIRS += surfacemesh_filter_au_skeleton
#SUBDIRS += surfacemesh_filter_geoheat
#SUBDIRS += surfacemesh_filter_ballpivoting

# Mode Plugins
SUBDIRS += surfacemesh_mode_info
#SUBDIRS += surfacemesh_mode_arapdeform
