#ifndef MUTANT_META_H_
#define MUTANT_META_H_

namespace mutant
{

namespace meta
{

	template<typename OutT, typename InT, class EvalT = mutant::linear_evaluator<InT> >
	struct interpolator_composite_3_to_1
	{
		typedef
			mutant::interpolator1<
				mutant::knot_data<InT,InT>,
				mutant::composite_evaluator
				<
					OutT,
					mutant::comp_3to1<OutT,InT,InT,InT>,
						EvalT, EvalT, EvalT
				>
			> type;
	};

	template<typename OutT, typename PosT, typename QuatT, typename SclT, typename InT, template<typename> class TimeAlgoT>
	struct hermite_separate_interpolator
	{
		typedef
			hermite_evaluator_cubic<InT> t_hermite_evaluator;

		typedef
			mutant::interpolator1<
				knot_data<
					float,
					InT
				>,
				t_hermite_evaluator,
				TimeAlgoT>
			t_hermite_interpolator;

		typedef
			mutant::composite_interpolator_small<
				PosT,
				comp_3to1<PosT, InT, InT, InT>,
				t_hermite_interpolator,
				t_hermite_interpolator,
				t_hermite_interpolator>
			t_position_interpolator;

		typedef
			mutant::composite_interpolator_small<
				QuatT,
				comp_quaternion_from_euler<QuatT, InT, InT, InT>,
				t_hermite_interpolator,
				t_hermite_interpolator,
				t_hermite_interpolator>
			t_rotation_interpolator;

		typedef
			mutant::composite_interpolator_small<
				PosT,
				comp_3to1<PosT, InT, InT, InT>,
				t_hermite_interpolator,
				t_hermite_interpolator,
				t_hermite_interpolator>
			t_scale_interpolator;

		typedef
			mutant::composite_interpolator_small<
				OutT,
				comp_3to1<OutT, PosT, QuatT, SclT>,
				t_position_interpolator,
				t_rotation_interpolator,
				t_scale_interpolator>
			t_transform_interpolator;

		typedef t_transform_interpolator
			type;

		template<class PosIT, class RotIT, class SclIT>
		struct custom_composites
		{
			typedef
				mutant::composite_interpolator_small<
					OutT,
					comp_3to1<OutT, PosT, QuatT, SclT>,
					PosIT,
					RotIT,
					SclIT>
				t_transform_interpolator;

			typedef t_transform_interpolator
				type;
		};
	};
}

}

#endif // MUTANT_META_H_
