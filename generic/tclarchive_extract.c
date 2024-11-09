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
 * This function is from libarchive example
 */
int copy_data(struct archive *ar, struct archive *aw)
{
	int r;
	const void *buff;
	size_t size;
	int64_t offset;

	for (;;) {
		r = archive_read_data_block(ar, &buff, &size, &offset);
		if (r == ARCHIVE_EOF)
			return (ARCHIVE_OK);
		if (r != ARCHIVE_OK) {
                        fprintf(stderr, "copy_data failed at %s\n", archive_error_string(ar));
			return (r);
		}
		r = archive_write_data_block(aw, buff, size, offset);
		if (r != ARCHIVE_OK) {
                        fprintf(stderr, "copy_data failed at %s\n", archive_error_string(ar));
			return (r);
		}
	}
}

/*
 *----------------------------------------------------------------------
 *
 * ARCHIVE_EXTRACT --
 *
 *	Extract an archive file.
 *
 *----------------------------------------------------------------------
 */
int ARCHIVE_EXTRACT(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int r;
    char *zArg = NULL;
    const char *zFile = NULL;
    Tcl_DString translatedFilename;
    const char *zFormat = NULL;
    const char *zFilter = NULL;
    int do_extract = 0;
    const char *zPath = NULL;
    char newPath[PATH_MAX];
    const char *oldPath = NULL;
    Tcl_Obj *pResultStr = NULL;
    Tcl_Size len;
    int i;
    int flags = ARCHIVE_EXTRACT_TIME;
    const char *zPasswd = NULL;

    if( objc<5 || (objc&1)!=1 ) {
        Tcl_WrongNumArgs(interp, 1, objv,
          "filename filter format doextractflag ?-path path? ?-fullpermissions boolean? \
?-nooverwrite boolean? ?-passwd password?"
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

    if( Tcl_GetBooleanFromObj(interp, objv[4], &do_extract) ) {
        return TCL_ERROR;
    }

    a = archive_read_new();

    if (strcmp(zFilter, "all")==0) {
        archive_read_support_filter_all(a);
    } else if (strcmp(zFilter, "bzip2")==0) {
        archive_read_support_filter_bzip2(a);
    } else if (strcmp(zFilter, "compress")==0) {
        archive_read_support_filter_compress(a);
    } else if (strcmp(zFilter, "gzip")==0) {
        archive_read_support_filter_gzip(a);
    } else if (strcmp(zFilter, "grzip")==0) {
        archive_read_support_filter_grzip(a);
    } else if (strcmp(zFilter, "lrzip")==0) {
        archive_read_support_filter_lrzip(a);
    } else if (strcmp(zFilter, "lzip")==0) {
        archive_read_support_filter_lzip(a);
    } else if (strcmp(zFilter, "lzma")==0) {
        archive_read_support_filter_lzma(a);
    } else if (strcmp(zFilter, "lzop")==0) {
        archive_read_support_filter_lzop(a);
    } else if (strcmp(zFilter, "none")==0) {
        archive_read_support_filter_none(a);
    } else if (strcmp(zFilter, "rpm")==0) {
        archive_read_support_filter_rpm(a);
    } else if (strcmp(zFilter, "uu")==0) {
        archive_read_support_filter_uu(a);
    } else if (strcmp(zFilter, "xz")==0) {
        archive_read_support_filter_xz(a);
    } else {
        archive_read_free(a);

        Tcl_AppendResult(interp, "filter string is wrong.", (char*)0);
        return TCL_ERROR;        
    }

    if (strcmp(zFormat, "all")==0) {
        archive_read_support_format_all(a);
    } else if (strcmp(zFormat, "7zip")==0) {
        archive_read_support_format_7zip(a);
    } else if (strcmp(zFormat, "ar")==0) {
        archive_read_support_format_ar(a);
    } else if (strcmp(zFormat, "cab")==0) {
        archive_read_support_format_cab(a);
    } else if (strcmp(zFormat, "cpio")==0) {
        archive_read_support_format_cpio(a);
    } else if (strcmp(zFormat, "gnutar")==0) {
        archive_read_support_format_gnutar(a);
    } else if (strcmp(zFormat, "iso9660")==0) {
        archive_read_support_format_iso9660(a);
    } else if (strcmp(zFormat, "lha")==0) {
        archive_read_support_format_lha(a);
    } else if (strcmp(zFormat, "mtree")==0) {
        archive_read_support_format_mtree(a);
    } else if (strcmp(zFormat, "rar")==0) {
        archive_read_support_format_rar(a);
    } else if (strcmp(zFormat, "tar")==0) {
        archive_read_support_format_tar(a);
    } else if (strcmp(zFormat, "xar")==0) {
        archive_read_support_format_xar(a);
    } else if (strcmp(zFormat, "zip")==0) {
        archive_read_support_format_zip(a);
    } else {
        archive_read_free(a);

        Tcl_AppendResult(interp, "format string is wrong.", (char*)0);
        return TCL_ERROR;        
    }

    for(i=5; i+1<objc; i+=2){
        zArg = Tcl_GetStringFromObj(objv[i], 0);

        if( strcmp(zArg, "-path")==0 ) {
            int check;
            zPath = Tcl_GetStringFromObj(objv[i+1], &len);
            if(!zPath || len < 0) {
                return TCL_ERROR;
            }

            Tcl_IncrRefCount(objv[i+1]);
            check = Tcl_FSAccess( objv[i+1], F_OK);
            Tcl_DecrRefCount(objv[i+1]);
            if(check!=0) {
                Tcl_AppendResult(interp, "path not exist!!!", (char*)0);
                return TCL_ERROR; 
            }
        } else if( strcmp(zArg, "-fullpermissions")==0 ) {
            int b;
            if( Tcl_GetBooleanFromObj(interp, objv[i+1], &b) ) return TCL_ERROR;

            if( b ){
              flags |= ARCHIVE_EXTRACT_PERM;
              flags |= ARCHIVE_EXTRACT_ACL;
              flags |= ARCHIVE_EXTRACT_FFLAGS;
            }else{
              flags &= ~ARCHIVE_EXTRACT_PERM;
              flags &= ~ARCHIVE_EXTRACT_ACL;
              flags &= ~ARCHIVE_EXTRACT_FFLAGS;
            }
        } else if( strcmp(zArg, "-nooverwrite")==0 ) {
            int b;
            if( Tcl_GetBooleanFromObj(interp, objv[i+1], &b) ) return TCL_ERROR;

            if( b ){
              flags |= ARCHIVE_EXTRACT_NO_OVERWRITE;
            }else{
              flags &= ~ARCHIVE_EXTRACT_NO_OVERWRITE;
            }
        } else if( strcmp(zArg, "-passwd")==0 ) {
            zPasswd = Tcl_GetStringFromObj(objv[i+1], &len);
            if(!zPasswd || len < 0) {
                return TCL_ERROR;
            }
        } else {
           Tcl_AppendResult(interp, "unknown option: ", zArg, (char*)0);
           return TCL_ERROR;
        }
    }
            
#if ARCHIVE_VERSION_NUMBER >= 3002000
    /*
     * I guess libarchive added support to decrypt some formats >= 3.2.0.
     * If user gives a password, try to add a decryption passphrase.
     */
    if(zPasswd != NULL) {
        archive_read_add_passphrase(a, zPasswd);
    }
#endif

    /*
     * Now try to read file
     */
    zFile = Tcl_TranslateFileName(interp, zFile, &translatedFilename);
    r = archive_read_open_filename(a, zFile, 16384);
    Tcl_DStringFree(&translatedFilename);
    if (r != ARCHIVE_OK) {
        Tcl_AppendResult(interp, "open file fail.", (char*)0);
        return TCL_ERROR;
    }

    /*
     * Now setup write options
     */
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);

    if (!do_extract) {
        pResultStr = Tcl_NewListObj(0, NULL);
    }

    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;

        if (r < ARCHIVE_OK) {
            fprintf(stderr, "%s\n", archive_error_string(a));
        }

        if (r < ARCHIVE_WARN)   {
            Tcl_AppendResult(interp, archive_error_string(a), (char*)0);
            return TCL_ERROR;
        }          

        oldPath = archive_entry_pathname(entry);
        if (!do_extract) {
            Tcl_ListObjAppendElement(interp, pResultStr, Tcl_NewStringObj(oldPath, -1 ));
        } else {
            /*
             * If user gives a path, then we uses it.
             */
            if(zPath != NULL) {   
                snprintf( newPath, PATH_MAX, "%s/%s", zPath, oldPath );
                archive_entry_set_pathname( entry, newPath );
            } else {
                snprintf( newPath, PATH_MAX, "%s", oldPath );
            }

            r = archive_write_header(ext, entry);
            if (r != ARCHIVE_OK) {
                fprintf(stderr, "%s\n", archive_error_string(a));
	    } else {
                r = copy_data(a, ext);
                if (r != ARCHIVE_OK) {
                    fprintf(stderr, "copy_data failed at %s\n", newPath);
                }

                r = archive_write_finish_entry(ext);
                if (r != ARCHIVE_OK) {
                    fprintf(stderr, "archive_write_finish_entry() %s\n", archive_error_string(ext));
                }
            }
        }

        oldPath = NULL;
        newPath[0] = '\0';
    }

    archive_read_close(a);
    archive_read_free(a);

    archive_write_close(ext);
    archive_write_free(ext);

    if (!do_extract) {
         Tcl_SetObjResult(interp, pResultStr);
    }
        
    return TCL_OK;
}

