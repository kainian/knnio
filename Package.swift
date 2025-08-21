// swift-tools-version: 6.1
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "npnio",
    products: [
        .library(
            name: "NPNIO",
            targets: [
                "NPNIO"
            ]),
    ],
    dependencies: [
        .package(url: "https://github.com/noctisplanet/npfoundation.git", branch: "main"),
    ],
    targets: [
        .target(
            name: "NPNIO",
            dependencies: [
                .product(name: "NPFoundation", package: "npfoundation")
            ]),
        .testTarget(
            name: "NPNIOTests",
            dependencies: ["NPNIO"]),
    ],
    cxxLanguageStandard: .cxx17
)
