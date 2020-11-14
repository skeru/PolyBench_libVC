#include "versioningCompiler/Version.hpp"

#ifdef HAVE_CLANG_LIB
#include "versioningCompiler/CompilerImpl/ClangLibCompiler.hpp"
#else
#include "versioningCompiler/CompilerImpl/SystemCompiler.hpp"
#endif

#include <iostream>
#include <list>
#include <string.h>

#ifndef POLYBENCH_SOURCE_DIRECTORY
#define POLYBENCH_SOURCE_DIRECTORY "../polybench-c-4.2.1-beta"
#endif
const std::string polybench_source = POLYBENCH_SOURCE_DIRECTORY;

typedef std::pair<std::string, vc::Option> list_element_t;
typedef std::pair<std::string, std::shared_ptr<vc::Version> > run_element_t;
typedef int (main_signature_t)(int argc, char*argv[]);

typedef struct kd_t{
	std::string kernel_label;
	std::string kernel_folder;
	std::string fileName;
	std::string function_name;
	std::list<list_element_t> data_type_option_list;
} kernel_descriptor_t;

typedef struct cat_t {
	std::string category_label;
	std::string category_folder;
	std::list<kernel_descriptor_t> kernel_config_list;
} category_t;

int main(int argc, char const *argv[]) {
	// common options
	const vc::opt_list_t default_options {
                vc::make_option("-includeutils.hpp"),
                vc::make_option("-O0"),
                vc::make_option("-I" + polybench_source + "/utilities"),
                vc::make_option("-I../include"),
	};

	// data type options
	const vc::opt_list_t data_type_opt = {
		vc::make_option("-DDATA_TYPE_IS_INT"),
		vc::make_option("-DDATA_TYPE_IS_FLOAT"),
		vc::make_option("-DDATA_TYPE_IS_DOUBLE"),
	};

	auto it = data_type_opt.begin();

	std::list<list_element_t> dataType_list_ifd = {
		std::make_pair("int", *it),
		std::make_pair("float", *(std::next(it, 1))),
		std::make_pair("double", *(std::next(it, 2))),
	};
	std::list<list_element_t> dataType_list_fd = {
		std::make_pair("float", *(std::next(it, 1))),
		std::make_pair("double", *(std::next(it, 2))),
	};

	// data size options
	const vc::opt_list_t data_size = {
		vc::make_option("-DMINI_DATASET"),
		vc::make_option("-DSMALL_DATASET"),
		vc::make_option("-DMEDIUM_DATASET"),
		vc::make_option("-DLARGE_DATASET"),
		vc::make_option("-DEXTRALARGE_DATASET"),
	};

	it = data_size.begin();

	std::list<list_element_t> size_list = {
		std::make_pair("mini", *it),
		std::make_pair("small", *(std::next(it, 1))),
		std::make_pair("medium", *(std::next(it, 2))),
		std::make_pair("large", *(std::next(it, 3))),
		std::make_pair("xlarge", *(std::next(it, 4))),
	};

	// create a compiler
	std::shared_ptr<vc::Compiler> default_compiler;
	#ifdef HAVE_CLANG_LIB
	default_compiler = std::make_shared<vc::ClangLibCompiler>("ClangLibCompiler",
	                                                          ".",
	                                                          "compilation.log");
	#else
	default_compiler = std::make_shared<vc::SystemCompiler>("default_gcc",
	                                                        "gcc",
	                                                        ".",
	                                                        "compilation.log");
	#endif

	// declare kernel
	std::list<category_t> configList;
	category_t category;
	kernel_descriptor_t k_tmp;
	k_tmp.function_name = "main"; // same for every kernel

	// datamining
	category.category_folder = "datamining";
	category.category_label = "dm";

	k_tmp.kernel_label = "correlation";
	k_tmp.kernel_folder = "correlation";
	k_tmp.fileName = "correlation.c";
	k_tmp.data_type_option_list = dataType_list_fd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "covariance";
	k_tmp.kernel_folder = "covariance";
	k_tmp.fileName = "covariance.c";
	k_tmp.data_type_option_list = dataType_list_fd;
	category.kernel_config_list.push_back(k_tmp);

	configList.push_back(category);

	// linear algebra
	category.category_folder = "linear-algebra";
	category.category_label = "la";
	category.kernel_config_list.clear();

	k_tmp.kernel_label = "blas/gemm";
	k_tmp.kernel_folder = "blas/gemm";
	k_tmp.fileName = "gemm.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "blas/gemver";
	k_tmp.kernel_folder = "blas/gemver";
	k_tmp.fileName = "gemver.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "blas/gesummv";
	k_tmp.kernel_folder = "blas/gesummv";
	k_tmp.fileName = "gesummv.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "blas/syr2k";
	k_tmp.kernel_folder = "blas/syr2k";
	k_tmp.fileName = "syr2k.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "blas/syrk";
	k_tmp.kernel_folder = "blas/syrk";
	k_tmp.fileName = "syrk.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "blas/trmm";
	k_tmp.kernel_folder = "blas/trmm";
	k_tmp.fileName = "trmm.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "ker/2mm";
	k_tmp.kernel_folder = "kernels/2mm";
	k_tmp.fileName = "2mm.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "ker/3mm";
	k_tmp.kernel_folder = "kernels/3mm";
	k_tmp.fileName = "3mm.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "ker/atax";
	k_tmp.kernel_folder = "kernels/atax";
	k_tmp.fileName = "atax.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "ker/bicg";
	k_tmp.kernel_folder = "kernels/bicg";
	k_tmp.fileName = "bicg.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "ker/doitgen";
	k_tmp.kernel_folder = "kernels/doitgen";
	k_tmp.fileName = "doitgen.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "ker/mvt";
	k_tmp.kernel_folder = "kernels/mvt";
	k_tmp.fileName = "mvt.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "solv/cholesky";
	k_tmp.kernel_folder = "solvers/cholesky";
	k_tmp.fileName = "cholesky.c";
	k_tmp.data_type_option_list = dataType_list_fd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "solv/durbin";
	k_tmp.kernel_folder = "solvers/durbin";
	k_tmp.fileName = "durbin.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "solv/gramschmidt";
	k_tmp.kernel_folder = "solvers/gramschmidt";
	k_tmp.fileName = "gramschmidt.c";
	k_tmp.data_type_option_list = dataType_list_fd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "solv/lu";
	k_tmp.kernel_folder = "solvers/lu";
	k_tmp.fileName = "lu.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "solv/ludcmp";
	k_tmp.kernel_folder = "solvers/ludcmp";
	k_tmp.fileName = "ludcmp.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "solv/trisolv";
	k_tmp.kernel_folder = "solvers/trisolv";
	k_tmp.fileName = "trisolv.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	configList.push_back(category);

	// medley
	category.category_folder = "medley";
	category.category_label = "med";
	category.kernel_config_list.clear();

	k_tmp.kernel_label = "deriche";
	k_tmp.kernel_folder = "deriche";
	k_tmp.fileName = "deriche.c";
	k_tmp.data_type_option_list = dataType_list_fd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "fl-war";
	k_tmp.kernel_folder = "floyd-warshall";
	k_tmp.fileName = "floyd-warshall.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "nussinov";
	k_tmp.kernel_folder = "nussinov";
	k_tmp.fileName = "nussinov.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	configList.push_back(category);

	// stencils
	category.category_folder = "stencils";
	category.category_label = "sten";
	category.kernel_config_list.clear();

	k_tmp.kernel_label = "adi";
	k_tmp.kernel_folder = "adi";
	k_tmp.fileName = "adi.c";
	k_tmp.data_type_option_list = dataType_list_fd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "fdtd-2d";
	k_tmp.kernel_folder = "fdtd-2d";
	k_tmp.fileName = "fdtd-2d.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "heat-3d";
	k_tmp.kernel_folder = "heat-3d";
	k_tmp.fileName = "heat-3d.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "jacobi-1d";
	k_tmp.kernel_folder = "jacobi-1d";
	k_tmp.fileName = "jacobi-1d.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "jacobi-2d";
	k_tmp.kernel_folder = "jacobi-2d";
	k_tmp.fileName = "jacobi-2d.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	k_tmp.kernel_label = "seidel-2d";
	k_tmp.kernel_folder = "seidel-2d";
	k_tmp.fileName = "seidel-2d.c";
	k_tmp.data_type_option_list = dataType_list_ifd;
	category.kernel_config_list.push_back(k_tmp);

	configList.push_back(category);

	// preparing versions
	vc::Version::Builder builder;
	std::list<run_element_t> jobList;

	for (const auto &size : size_list) {
		for (const auto &cat : configList) {
			for (const auto &k : cat.kernel_config_list) {
				for (const auto &datatype : k.data_type_option_list) {
					builder.reset();
					const std::string label = cat.category_label + " " +
					                          k.kernel_label + " - " +
					                          size.first + " - " +
					                          datatype.first;
					builder._compiler = default_compiler;
					const std::string kernelSourceDir = polybench_source + "/" +
					                                    cat.category_folder + "/" +
					                                    k.kernel_folder;
					builder.addIncludeDir(kernelSourceDir);
					vc::opt_list_t options = {
						size.second,
						datatype.second,
					};
					options.insert(options.end(), default_options.begin(), default_options.end());
					builder.options(options);
					builder.addSourceFile(kernelSourceDir + "/" + k.fileName);
					builder.addSourceFile(polybench_source + "/utilities/polybench.c");
					builder._functionName = {k.function_name};
					jobList.push_back(std::make_pair(label, builder.build()));
				}
			}
		}
	}

	// compiling and running
	char* runningArgv[] = {};
	for (const auto &j : jobList) {
		std::cout << "working on " << j.first << std::endl;
		bool ok = j.second->compile();
		if (!ok) {
			std::cerr << "Error while compiling " << j.first << std::endl;
			continue;
		}
		main_signature_t* run = reinterpret_cast<main_signature_t*>(j.second->getSymbol());
		if (run) {
			run(0, runningArgv);
		}
		j.second->fold();
	}

	return 0;
}
