import java.util.Arrays;
import java.util.stream.LongStream;
import java.util.Random;

public class Test {
	private static void printResult(int[] result) {
		System.out.print("[(" + result[0] + ",0)");
		for (int i = 1; i < result.length; ++i)
			System.out.print(", (" + result[i] + "," + i + ")");
		System.out.print("]\n");
	}

	private static void specificTests() {
		System.out.println("== Specific Tests ==\n");

		double[][][] matrices = {
				{
					{0, 0},
					{0, 0}
				},

				{
					{0, 0, 0, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 1}
				},

				{
					{0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0},
					{0, 0, 1, 1, 1},
					{0, 0, 1, 1, 1},
					{0, 0, 1, 1, 1}
				}
			};

		for (int i = 0; i < matrices.length; ++i) {
			HungarianAlgorithm X = new HungarianAlgorithm(matrices[i]);
			int[] result = X.execute();
			System.out.println("Test " + (i + 1));
			printResult(result);
		}
	}

	private static void speedTest(long todo, long size) {
		final long THOUSAND = 1000;
		final long MILLION = THOUSAND * THOUSAND;
		final long BILLION = MILLION * THOUSAND;

		System.out.println("== Speed Test (" + todo + ' ' + size + 'x' + size + ") ==\n");

		Random rand = new Random();
		long totalTime = 0;

		for (long total = 1; total <= todo; ++total) {
			double[][] matrix = LongStream.range(0,size).mapToObj(i -> rand.longs(size,0,size*size).asDoubleStream().toArray()).toArray(double[][]::new);

			// System.out.println(Arrays.deepToString(matrix).replace("], ", "]\n "));

			if (total < 12) System.out.println(total);
			else if (total < 100 && total % 12 == 0) System.out.println(total / 12 + " Dozen");
			else if (total < THOUSAND && total % 100 == 0) System.out.println(total / 100 + " Hundred");
			else if (total < MILLION && total % THOUSAND == 0) System.out.println(total / THOUSAND + " Thousand");
			else if (total < BILLION && total % MILLION == 0) System.out.println(total / MILLION + " Million");
			else if (total % BILLION == 0) System.out.println(total / BILLION + " Billion");

			HungarianAlgorithm X = new HungarianAlgorithm(matrix);
			long start = System.nanoTime();
			int[] result = X.execute();
			long end = System.nanoTime();
			totalTime += end - start;

			// printResult(result);
		}

		System.out.println("\n\n" + (((double)totalTime / (double)todo) / 1000000000.d) + "s Average Time\n\n");
	}

	public static void main(String args[]) {
		specificTests();
		speedTest(10000,50);
		speedTest(100,250);
		speedTest(10,1000);
	}
}
