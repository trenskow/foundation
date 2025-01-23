// swift-tools-version:5.9

//
// Package.swift
// foundation-cpp
//
// Created by Kristian Trenskow on 2023/10/07
// See license in LICENSE.
//

import PackageDescription

let package = Package(
	name: "FoundationCxx",
	products: [
		.library(
			name: "FoundationCxx",
			targets: ["FoundationCxx"])
	],
	targets: [
		.target(
			name: "FoundationCxx",
			path: ".",
			exclude: [
				"README.md",
				"LICENSE"
			],
			publicHeadersPath: "./include",
			swiftSettings: [
				.interoperabilityMode(.Cxx)
			]
		)
	],
	cxxLanguageStandard: .cxx17
)
