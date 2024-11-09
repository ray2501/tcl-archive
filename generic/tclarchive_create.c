/*
 * Copyright (c) <2018>, <Danilo Chang>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "tclarchive.h"

/*
 *----------------------------------------------------------------------
 *
 * ARCHIVE_CREATE --
 *
 *      Create an archive file.
 *
 *----------------------------------------------------------------------
 */
int ARCHIVE_CREATE(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    struct archive *a;
    struct archive_entry *entry;
    FILE *fp;
    int filelength = 0;
    char buff[16384];
    int r;
    const char *zFile = NULL;
    Tcl_DString translatedFilename;
    const char *zFormat = NULL;
    const char *zFilter = NULL;
    Tcl_Size len = 0, filesListCount = 0;
    int count = 0, count2 = 0;
    Tcl_Obj *pFilesList = NULL;
    char **argv = NULL;

    if( objc != 5 ) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "filename filter format filesList"
        );

        return TCL_ERROR;
    }

    zFile = Tcl_GetStringFromObj(objv[1], &len);
    if( !zFile || len < 1 ) {
        return TCL_ERROR;
    }

    zFilter = Tcl_GetStringFromObj(objv[2], &len);
    if( !zFilter || len < 1 ) {
        return TCL_ERROR;
    }

    zFormat = Tcl_GetStringFromObj(objv[3], &len);
    if( !zFormat || len < 1 ) {
        return TCL_ERROR;
    }

    pFilesList = objv[4];
    if(Tcl_ListObjLength(interp, pFilesList, &filesListCount)) {
        return TCL_ERROR;
    }

    /*
     * Now do the ugly thing: convert Tcl List to a (char **) string array
     */
    argv = (char **) malloc (sizeof(char *) * (filesListCount + 1));

    if(!argv) {
        Tcl_AppendResult(interp, "malloc memory fail.", (char*)0);
        return TCL_ERROR;        
    }

    for(count = 0; count <= filesListCount; count++) {
       argv[count] = NULL;
    }

    for(count = 0; count < filesListCount; count++) {
        Tcl_Obj *pFileListElement = NULL;
        char *filepath = NULL;
        Tcl_Size iLength = 0;

        if(Tcl_ListObjIndex(interp, pFilesList, count, &pFileListElement)) {
            if(argv) free(argv);

            Tcl_AppendResult(interp, "Get list element fail.", (char*)0);
            return TCL_ERROR;        
        }

        filepath = Tcl_GetStringFromObj(pFileListElement, &iLength);
        if( !filepath || iLength < 1 ) {
            if(argv) free(argv);

            Tcl_AppendResult(interp, "Get list element string fail.", (char*)0);
            return TCL_ERROR;
        }

        argv[count] = (char *) malloc (sizeof (char) * (iLength + 1));
        strcpy(argv[count], filepath);
    }

    /*
     * Now try to create an achive file
     */
    a = archive_write_new();

    if (strcmp(zFilter, "b64encode")==0) {
        archive_write_add_filter_b64encode(a);
    } else if (strcmp(zFilter, "bzip2")==0) {
        archive_write_add_filter_bzip2(a);
    } else if (strcmp(zFilter, "compress")==0) {
        archive_write_add_filter_compress(a);
    } else if (strcmp(zFilter, "gzip")==0) {
        archive_write_add_filter_gzip(a);
    } else if (strcmp(zFilter, "grzip")==0) {
        archive_write_add_filter_grzip(a);
    } else if (strcmp(zFilter, "lrzip")==0) {
        archive_write_add_filter_lrzip(a);
    } else if (strcmp(zFilter, "lzip")==0) {
        archive_write_add_filter_lzip(a);
    } else if (strcmp(zFilter, "lzma")==0) {
        archive_write_add_filter_lzma(a);
    } else if (strcmp(zFilter, "lzop")==0) {
        archive_write_add_filter_lzop(a);
    } else if (strcmp(zFilter, "none")==0) {
        archive_write_add_filter_none(a);
    } else if (strcmp(zFilter, "uuencode")==0) {
        archive_write_add_filter_uuencode(a);
    } else if (strcmp(zFilter, "xz")==0) {
        archive_write_add_filter_xz(a);
    } else {
        archive_write_free(a);

        Tcl_AppendResult(interp, "filter string is wrong.", (char*)0);
        return TCL_ERROR;        
    }

    if (strcmp(zFormat, "7zip")==0) {
        archive_write_set_format_7zip(a);
    } else if (strcmp(zFormat, "ar_bsd")==0) {
        archive_write_set_format_ar_bsd(a);
    } else if (strcmp(zFormat, "ar_srv4")==0) {
        archive_write_set_format_ar_svr4(a);
    } else if (strcmp(zFormat, "cpio")==0) {
        archive_write_set_format_cpio(a);
    } else if (strcmp(zFormat, "cpio_newc")==0) {
        archive_write_set_format_cpio_newc(a);
    } else if (strcmp(zFormat, "gnutar")==0) {
        archive_write_set_format_gnutar(a);
    } else if (strcmp(zFormat, "iso9660")==0) {
        archive_write_set_format_iso9660(a);
    } else if (strcmp(zFormat, "mtree")==0) {
        archive_write_set_format_mtree(a);
    } else if (strcmp(zFormat, "mtree_classic")==0) {
        archive_write_set_format_mtree_classic(a);
    } else if (strcmp(zFormat, "pax")==0) {
        archive_write_set_format_pax(a);
    } else if (strcmp(zFormat, "pax_restricted")==0) {
        archive_write_set_format_pax_restricted(a);
    } else if (strcmp(zFormat, "shar")==0) {
        archive_write_set_format_shar(a);
    } else if (strcmp(zFormat, "shar_dump")==0) {
        archive_write_set_format_shar_dump(a);
    } else if (strcmp(zFormat, "ustar")==0) {
        archive_write_set_format_ustar(a);
    } else if (strcmp(zFormat, "v7tar")==0) {
        archive_write_set_format_v7tar(a);
    } else if (strcmp(zFormat, "xar")==0) {
        archive_write_set_format_xar(a);
    } else if (strcmp(zFormat, "zip")==0) {
        archive_write_set_format_zip(a);
    } else {
        archive_write_free(a);

        Tcl_AppendResult(interp, "format string is wrong.", (char*)0);
        return TCL_ERROR;        
    }

    zFile = Tcl_TranslateFileName(interp, zFile, &translatedFilename);
    r = archive_write_open_filename(a, zFile);
    Tcl_DStringFree(&translatedFilename);
    if (r != ARCHIVE_OK) {
        Tcl_AppendResult(interp, "open file fail.", (char*)0);
        return TCL_ERROR;
    }

    for(count2 = 0; count2 < filesListCount; count2++) {
        struct archive *disk = archive_read_disk_new();
        archive_read_disk_set_standard_lookup(disk);

        r = archive_read_disk_open(disk, argv[count2]);
        if (r != ARCHIVE_OK) {
            /* Free argv memory before quit */
            for(count = 0; count < filesListCount; count++) {
                if(argv[count]) free(argv[count]);
            }

            if(argv) free(argv);
            Tcl_AppendResult(interp, archive_error_string(disk), (char*)0);
            return TCL_ERROR;
	}

        for (;;) {
            entry = archive_entry_new();
            r = archive_read_next_header2(disk, entry);
            if (r == ARCHIVE_EOF)
                break;

            if (r != ARCHIVE_OK) {
                /* Free argv memory before quit */
                for(count = 0; count < filesListCount; count++) {
                    if(argv[count]) free(argv[count]);
                }

                if(argv) free(argv);

                Tcl_AppendResult(interp, archive_error_string(disk), (char*)0);
                return TCL_ERROR;
            }

            archive_read_disk_descend(disk);
            r = archive_write_header(a, entry);
            if (r < ARCHIVE_OK) {
                fprintf(stdout, "%s\n", archive_error_string(a));
            }

            if (r == ARCHIVE_FATAL) {
                /* Free argv memory before quit */
                for(count = 0; count < filesListCount; count++) {
                    if(argv[count]) free(argv[count]);
                }

                if(argv) free(argv);
                Tcl_AppendResult(interp, archive_error_string(a), (char*)0);
                return TCL_ERROR;
            }

            if (r > ARCHIVE_FAILED) {
                fp = fopen(archive_entry_sourcepath(entry), "r");
                if(fp == NULL) {
                   fprintf(stderr, "fopen failed.\n");
                   break;
                }

                filelength = fread(buff, 1, sizeof(buff), fp);
                while (filelength > 0) {
                    archive_write_data(a, buff, filelength);
                    filelength = fread(buff, 1, sizeof(buff), fp);
                }

                fclose(fp);
            }

            archive_entry_free(entry);
        }

     	archive_read_close(disk);
	archive_read_free(disk);
    }

    /* free memory for argv */
    for(count = 0; count < filesListCount; count++) {
        if(argv[count]) free(argv[count]);
    }

    if(argv) free(argv);

    archive_write_close(a);
    archive_write_free(a);

    return TCL_OK;
}

