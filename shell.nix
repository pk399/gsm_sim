with (import <nixpkgs> {});
mkShell {
  buildInputs = [
    gcc
    gnumake
    libosmocore

    (python3.withPackages (python-pkgs: with python-pkgs; [
      numpy
      matplotlib
      tqdm
    ]))
  ];
}

