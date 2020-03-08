#pragma once

void test_cpu_restart();
void test_cpu_adc_immediate();
void test_cpu_init();

// Branch test
void test_cpu_branch_instructions();
void test_cpu_bcc();
void test_cpu_bcs();
void test_cpu_beq();
void test_cpu_bmi();
void test_cpu_bne();
void test_cpu_bpl();
void test_cpu_bvc();
void test_cpu_bvs();

// Load test
void test_cpu_load_instructions();
void test_cpu_lda_abs();
void test_cpu_lda_abs_x();
void test_cpu_lda_abs_y();
void test_cpu_lda_imm();
void test_cpu_lda_ind_x();
void test_cpu_lda_ind_y();
void test_cpu_lda_zpg();
void test_cpu_lda_zpg_x();

void test_cpu_ldx_abs();
void test_cpu_ldx_abs_y();
void test_cpu_ldx_imm();
void test_cpu_ldx_zpg();
void test_cpu_ldx_zpg_y();

void test_cpu_ldy_abs();
void test_cpu_ldy_abs_x();
void test_cpu_ldy_imm();
void test_cpu_ldy_zpg();
void test_cpu_ldy_zpg_x();

// Store test
void test_cpu_store_instructions();
void test_cpu_sta_abs();
void test_cpu_sta_abs_x();
void test_cpu_sta_abs_y();
void test_cpu_sta_ind_x();
void test_cpu_sta_ind_y();
void test_cpu_sta_zpg();
void test_cpu_sta_zpg_x();

void test_cpu_stx_abs();
void test_cpu_stx_zpg();
void test_cpu_stx_zpg_y();

void test_cpu_sty_abs();
void test_cpu_sty_zpg();
void test_cpu_sty_zpg_x();