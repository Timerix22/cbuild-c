## cbuild logic description
+ verify cbuild version
+ foreach dep in import
    + if dep starts with "./"
        + import local file
    + else 
        + if find(dep in ./cbuild)
        + else if find (dep in global_config_dir/**)
        + else error: "import target 'dep' not found, try 'fluzm find dep'"
+ apply proj settings
+ apply platform settings
+ apply configuration settings
    + apply platform settings
+ foreach task in tasks
    + apply platform settings
    + foreach tool in tool_order
        + apply task settings
        + foreach src in find(dir, langs)
            + if platform, settings, src or src_deps (included headers) were changed
                + src -> src_to_process
        + if parallel
            + foreach src in src_to_process
                + run tool on src
        + else
            + run tool on src_to_process
+ copy files to general_out_dir

## Example of project.dtsod:
```yml
cbuild_version: 0;
import: [ "c", "c++", "gcc", "./some_local_file.dtsod" ];

gcc: {
    src_languages: [ "c" ],
    src_dirs: [ "src" ],
};

configurations: {
    release: {
        preprocess_sources: {
            src_languages: [ "c", "c++" ],
            src_dirs: [ "src" ],
        },
        gcc: {
            pre_args: [ "-O2" ],
            post_args: [ "-Wl,--gc-sections" ],
        };
    };
}

tasks: {
    exe: {
        pre_tasks: [  ],
        tool_order: [ "preprocess_sources", "gcc", "g++", "g++-link" ],
        g++: [ ... ],
    };
};

languages: [
    {
        aliases: [ "c" ],
        file_extensions: [ "c" ],
    },
    {
        aliases: [ "c-header" ],
        file_extensions: [ "h" ],
    }
];

tools: [
    {
        aliases: [ "gcc" ],
        exe_file: [ "gcc" ],
        supported_languages: [ "c" ]; # set to "any" to use with any lang
        parallel: true,
    }
];
```
