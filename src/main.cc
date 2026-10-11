#include "app/cartty.hh"


auto main(int argc, char **argv) -> int
{ return cartty::app::application::run({ argv, std::size_t(argc) }); }
