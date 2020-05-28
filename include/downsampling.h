#ifndef _DOWNSAMPLING_H_
#define _DOWNSAMPLING_H_

#include <stdint.h>

/* type: int16_t**, uint8_t, uint8_t, int16_t**
 * rtype: void
 * Réalise le downsampling pour la composante Y sur un bloc 8x8
 * et stocke son résultat dans data_unit
 */
extern void process_Y(int16_t** Y_mcu, uint8_t hy, uint8_t vy,
                      int16_t** data_unit);

/* type: int16_t**, uint8_t, uint8_t, uint8_t, uint8_t, int16_t**
 * rtype: void
 * Réalise le downsampling pour les composantes chromatiques
 * sur un bloc 8x8 et stocke son résultat dans data_unit
 */
extern void process_chroma(int16_t** chroma_mcu, uint8_t hy, uint8_t vy,
                           uint8_t h_chroma, uint8_t v_chroma,
                           int16_t** data_unit);

#endif
