{ nixpkgs ? import <nixpkgs> {}
, stdenv ? nixpkgs.stdenv
, bison ? nixpkgs.bison
, lib ? nixpkgs.lib
, flex ? nixpkgs.flex
, python3 ? nixpkgs.python3
}:

stdenv.mkDerivation rec {
  name = "btcs-${version}";
  version = "0.1";

  src = ./.;

  makeFlags = ["PREFIX=$(out)"];

  nativeBuildInputs = [ bison flex python3 ];

  patchPhase = ''
    patchShebangs mph-opcodes
  '';

  meta = with lib; {
    description = "Bitcoin script evaluator";
    homepage = "https://github.com/jb55/btcs";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.mit;
  };
}
