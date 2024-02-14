{ pkgs ? import <nixpkgs> {} } :
  pkgs.mkShell {
    packages = with pkgs; [
      gnat13
      cmake
      gnumake
      flamegraph
      linuxKernel.packages.linux_6_1.perf
      bashInteractiveFHS 
      hotspot
      #libsForQt5.qt5.full 
      #libsForQt5.qt5.qttools
      #glxinfo # debuggin the opengl libraries and os.
      (python3.withPackages (ps : with ps;
      [
        gdal
        numpy
      ]))
    ];
} 
