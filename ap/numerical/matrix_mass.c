void ap_matrix_mass(double* restrict C, double* restrict B,
                    double* restrict ref_functions, double* restrict weights,
                    LAPACKINDEX num_points, double* restrict mass)
{

        int i;
        double functions[21*num_points];
        double functions_scaled[21*num_points];
        double weights_scaled[num_points];

        /* stuff for DGEMM. */
        LAPACKINDEX i_twentyone = 21;

        const double jacobian = fabs(B[ORDER(0, 0, 2, 2)]*B[ORDER(1, 1, 2, 2)] -
                                     B[ORDER(0, 1, 2, 2)]*B[ORDER(1, 0, 2, 2)]);

        ap_global_functions(C, ref_functions, num_points, functions);

        /* scale the weights by the jacobian. */
        for (i = 0; i < num_points; i++) {
                weights_scaled[i] = weights[i]*jacobian;
        }

        memcpy(functions_scaled, functions, sizeof(double)*(21*num_points));

        /*
         * scale the first set of function values by the weights and
         * determinant. Then perform matrix multiplication.
         */
        multiply_by_diagonal(21, num_points, weights_scaled, functions_scaled);
        DGEMM_WRAPPER_NT(i_twentyone, i_twentyone, num_points,
                         functions_scaled, functions, mass);
}
