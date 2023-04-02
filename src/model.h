#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "tensor.h"
#include "distributions.h"

typedef unsigned char element_t;

extern element_t* reorder_buffer;

typedef struct {
    size_t pad_zeros;
    size_t num_inputs_total;
    size_t num_classes;

    size_t* input_order; // of shape (#Inputs), with elements in [0; num_inputs_total)

    size_t num_filters;
    size_t filter_inputs;
    size_t filter_entries;
    size_t filter_hashes;
    
    matrix_t hash_parameters; // of shape (#Hashes, #Inputs)
    unsigned char bleach;

    tensor3d_t data; // of shape (#Discriminators, #Filters, #Entries)
} model_t;

/**
 * @brief Initialize a model.
 * 
 * @param model An empty model, assumed to be not NULL.
 * @param num_inputs The total number of inputs to the model
 * @param num_classes The number of distinct possible outputs of the model; the number of classes in the dataset
 * @param filter_inputs The number of boolean inputs to each LUT/filter in the model
 * @param filter_entries The size of the underlying storage arrays for the filters. Must be a power of two.
 * @param filter_hashes The number of hash functions for each filter.
 */
void model_init(model_t* model, size_t num_inputs, size_t num_classes, size_t filter_inputs, size_t filter_entries, size_t filter_hashes);

/**
 * @brief Initializes all the buffers in the model. Assumes that the right field values are already attributed (in num_inputs, num_classes, ...)
 * 
 */
void model_init_buffers(model_t* model);

/**
 * @brief Performs an inference with the provided input
 * 
 * @param model A initialized model
 * @param input A vector of boolean values representing the input sample
 * @return uint64_t 
 */
uint64_t model_predict(model_t* model, element_t* input); 

/**
 * @brief Performs a training step (updating filter values) for all discriminators
 * @param model An initialized model
 * @param input A vector of boolean values representing the input sample
 * @param target The class of the input
 */
void model_train(model_t* model, element_t* input, uint64_t target);

/**
 * @brief Performs an inference to generate a response (number of filters which return True)
 * 
 * @param model An initialized model
 * @param discriminator_index The index of the discriminator to perform prediction on
 * @param input A vector of boolean values representing the input sample
 * @return uint64_t 
 */
uint64_t discriminator_predict(model_t* model, size_t discriminator_index, element_t* input);

/**
 * @brief Performs a training step (updating filter values)
 * 
 * @param model The model to train
 * @param discriminator_index The index of the discriminator in the model passed
 * @param input A vector of boolean values representing the input sample
 */
void discriminator_train(model_t* model, size_t discriminator_index, element_t* input);

/**
 * @brief Check whether a value is a member of this filter (i.e. possibly seen at least b times)
 * 
 * @param model A initialized model, assumed to be not NULL.
 * @param discriminator_index The index of the discriminator
 * @param filter_index The index of the filter
 * @param input The bitvector to check the membership of
 * 
 * @return 1 if input is maybe a member of filter and 0 if input is definitely not a member of filter
 */
uint64_t filter_check_membership(model_t* model, size_t discriminator_index, size_t filter_index, element_t* input);

/**
 * @brief 
 * 
 * @param model The model to train
 * @param discriminator_index The index of the discriminator in the model passed (the class of input)
 * @param filter_index The filter index to add a member to
 * @param input A bitvector
 */
void filter_add_member(model_t* model, size_t discriminator_index, size_t filter_index, element_t* input);

#endif