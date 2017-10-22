//
//  FileArchive.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "FileArchive.hpp"
#include "FileHelper.hpp"
#include "FileReader.hpp"
#include "miniz.h"

using namespace Pocket;

bool FileArchive::TryCreateArchiveFile(const std::string &path, const std::string &archiveFile, const std::function<std::string(const std::string&)> &onFileParsed) {
    
    mz_zip_archive zipArchive;
    memset(&zipArchive, 0, sizeof(zipArchive));
    
    mz_bool succes = mz_zip_writer_init_file(&zipArchive, archiveFile.c_str(), 1);
    if (!succes) {
        return false;
    }
    
    bool wasAnyErrorAddedFiles = false;
    
    FileHelper::RecurseFolder(path, [&] (const std::string& path) {
        if (wasAnyErrorAddedFiles) return;
        std::string id = onFileParsed(path);
        if (id!="") {
            id = "F" + id; // if id starts with a '/' it will fail to add
            mz_bool canAddFile = mz_zip_writer_add_file(&zipArchive, id.c_str(), path.c_str(), "", 0, MZ_UBER_COMPRESSION);
            if (!canAddFile) {
                wasAnyErrorAddedFiles = true;
            }
        }
    });
    
    if (wasAnyErrorAddedFiles) {
        mz_zip_writer_end(&zipArchive);
        return false;
    }

    succes = mz_zip_writer_finalize_archive(&zipArchive);
    if (!succes) {
        mz_zip_writer_end(&zipArchive);
        return false;
    }

    succes = mz_zip_writer_end(&zipArchive);
    if (!succes) {
        return false;
    }

    return true;
}

FileArchive::FileArchive() {
    archive = new mz_zip_archive();
    memset(archive, 0, sizeof(mz_zip_archive));
}
FileArchive::~FileArchive() {
    mz_zip_archive* a = (mz_zip_archive*)archive;
    delete a;
}

bool FileArchive::Initialize(const std::string &path) {
    this->path = FileReader::GetFile(path);
    
    mz_zip_archive* zipArchive = (mz_zip_archive*)archive;
    
    bool succes = mz_zip_reader_init_file(zipArchive, this->path.c_str(), 0);
    if (!succes) {
        return false;
    }
    int numberOfFiles = (int)mz_zip_reader_get_num_files(zipArchive);
    
    for (int i=0; i<numberOfFiles; ++i) {
        mz_zip_archive_file_stat stats;
        memset(&stats, 0, sizeof(mz_zip_archive_file_stat));
        succes = mz_zip_reader_file_stat(zipArchive, i, &stats);
        if (!succes) {
            return false;
        }
        std::string id(stats.m_filename);
        id = id.substr(1, id.size() - 1);
        files[id] = { i, id, static_cast<size_t>(stats.m_comp_size), static_cast<size_t>(stats.m_uncomp_size) };
    }
    return true;
}

bool FileArchive::TryLoadData(const std::string& id, const std::function<void(void*, size_t)>& onData) {
    auto it = files.find(id);
    if (it == files.end()) {
        return false;
    }
    
    mz_zip_archive* zipArchive = (mz_zip_archive*)archive;
    
    size_t bufferSize;
    void* buffer = mz_zip_reader_extract_to_heap(zipArchive, (mz_uint)it->second.index, &bufferSize, 0);
    if (!buffer) {
        return false;
    }

    onData(buffer, bufferSize);
    return true;
}

const FileArchive::Files& FileArchive::GetFiles() { return files; }
