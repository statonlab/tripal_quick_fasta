<?php

/**
 * Form for downloading FASTA files.
 *
 * @param $form
 * @param $form_state
 */
function tripal_quick_fasta_form($form, &$form_state) {

  $feature_type_options = tripal_quick_fasta_fetch_features_for_sequence();

  $form['feature_type'] = [
    '#type' => 'select',
    '#title' => "Feature type for sequence",
    '#description' => 'The sequences will come from this feature type.',
    '#options' => $feature_type_options,
  ];

  $selected_type = $form_state['values']['feature_type'];


  $peptide_term = chado_get_cvterm(['id' => 'SO:0000104']);

  if ($selected_type === $peptide_term->cvterm_id){




    $form['header_select'] = [
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#prefix' => '<div id = "quick-fasta-header-select">',
      '#suffix' => '</div>',
    ];


    $form['header_select']['headers'] = [
      '#type' => 'checkbox',
      '#title' => 'Use the headers from mRNA',
      '#description' => 'The headers can come from a related feature type.',
    ];



    $form['header_select']['related_feature'] = [
      '#type' => 'select',
      '#title' => 'title',
      '#description' => 'description',
      '#options' => $feature_type_options
    ];


  }


}

