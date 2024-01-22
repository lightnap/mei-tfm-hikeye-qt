{ pkgs ? import <nixpkgs> {} } :
  pkgs.mkShell {
    packages = with pkgs; [
      gnat13
      cmake
      gnumake
      bashInteractiveFHS 
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
