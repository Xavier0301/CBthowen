#include "batch.h"

void batch_hashing(tensor3d_t* resulting_hashes, model_t* model, bmatrix_t* input_batch, size_t batch_size) {
    matrix_t tmp_hashes = { .stride = model->num_filters, .data=NULL };
    for(size_t it = 0; it < batch_size; ++it) {
        tmp_hashes.data = TENSOR3D_AXIS1(*resulting_hashes, it);
        perform_hashing(tmp_hashes, model, MATRIX_AXIS1(*input_batch, it));
    }
}

void batch_prediction(size_t* results, model_t* model, bmatrix_t* input_batch, size_t batch_size) {

}
