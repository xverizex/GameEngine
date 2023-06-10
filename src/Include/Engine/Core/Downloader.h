#ifndef ENGINE_DOWNLOADER_HEADER_H
#define ENGINE_DOWNLOADER_HEADER_H

#include <cstdint>
#include <vector>
#include <Engine/Core/VertexData.h>
#include <Engine/Core/InfoSprite.h>

void downloader_clear_memory_of_sprites ();
void downloader_init_sprite_array (uint32_t count);
void downloader_download_all_sprites_without_progressbar (std::vector<InfoSprite>& list);
void downloader_download_all_sprites_without_progressbar_by_range (uint32_t min, uint32_t max);
void downloader_download_each_sprite (InfoSprite& item);

VertexData *downloader_object_load (uint32_t asset);
VertexData *downloader_load_sprite (uint32_t asset);

#endif
