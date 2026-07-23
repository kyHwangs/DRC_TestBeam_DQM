# TB2022 DRC
Repository for GEANT4 simulation &amp; analysis of the dual-readout calorimeter for TB2022.

## How-to
### Compile
After fetching the repository, do
    
    cp envset.sh ./install
    cd install
    source envset.sh
    cd ../build
    cmake -DCMAKE_INSTALL_PREFIX=<path_to_install_directory> ..
    make -j4 install

to be updated
