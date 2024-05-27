{ pkgs ? import <nixpkgs> {} }:



pkgs.mkShell {
  packages = with pkgs; [
    gnat13
    cmake 
    gnumake 
    bashInteractiveFHS 
    gdb
    git
    gdal
    expat
    bzip2
    swig
    boost175
    libosmium
    (vscode-with-extensions.override
    {
      vscodeExtensions = with vscode-extensions;
      [
        ms-vscode.cpptools
        ms-vscode.cmake-tools
        ms-python.python
        ms-pyright.pyright
        ms-python.vscode-pylance
      ];
    })
    (python3.withPackages (ps : with ps; 
    [
      osmnx
      gpxpy
    ]))
  ];
} 
