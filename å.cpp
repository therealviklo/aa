#include <cstdio>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "module.h"
#include "deftypes.h"

int main(int argc, char* argv[])
{
	try
	{
		std::string in;
		std::string out;
		std::string triple = llvm::sys::getDefaultTargetTriple();
		for (int i = 1; i < argc; i++)
		{
			auto adv = [&](){
				if (++i >= argc)
					throw std::runtime_error("Förväntade kommandoradsargument.");
			};
			if (argv[i][0] == '-')
			{
				switch (argv[i][1])
				{
					case 'o':
					{
						adv();
						out = argv[i];
					}
					break;
					case 't':
					{
						adv();
						triple = argv[i];
					}
					break;
				}
			}
			else
			{
				in = argv[i];
			}
		}

		if (in.empty())
			throw std::runtime_error("Ingen infil angavs.");
		if (out.empty())
			throw std::runtime_error("Ingen utfil angavs.");

		Parser p(in, std::make_shared<std::set<fs::path>>());
		Scopes s;
		Context c;
		c.c = std::make_unique<llvm::LLVMContext>();
		c.builder = std::make_unique<llvm::IRBuilder<>>(*c.c);
		c.mod = std::make_unique<llvm::Module>(out, *c.c);
		initModule(triple, c);
		addDefaultTypes(c, s.tscope);
		p.parseFile(s);
		genModule(c, s);
		std::error_code err;
		llvm::raw_fd_ostream os(out, err);
		if (err)
			throw std::runtime_error(err.message());
		c.mod->print(os, nullptr);
	}
	catch (const std::exception& e)
	{
		std::printf("Fel: %s\n", e.what());
		return 1;
	}
	catch (...)
	{
		std::puts("Fel: Okänt fel");
		return 1;
	}
}