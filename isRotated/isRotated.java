class isRotated{
	static boolean checkRotation(String s1, String s2) {
		if (s1.length() == s2.length()) {
			for (int r = 0; r < s1.length(); r++) {
				for (int i = 0; i < s1.length(); i++) {
					if (s1.charAt(i) != s2.charAt((i + r) % s1.length())) break;
					else if (i == s1.length() - 1) return true;
				}
			}
		}

		return false;
	}

	public static void main(String[] args) {
		if (args.length < 2) {
			System.out.println("Requires 2 strings as arguments.");
			System.exit(1);
		}

		System.out.println(checkRotation(args[0], args[1]) ? "true" : "false");
	}
}