/*
 * Copyright (C) 2020  Jordi Sánchez
 * This file is part of CPM Hub
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <cest/cest.h>

#include <cpm/http/Endpoint.h>
#include <cpm/http/HttpParameterMap.h>


describe("Endpoint", []() {
    it("can be created from plain path", []() {
        Endpoint endpoint("/plain_path");
    });

    it("returns false when plain path doesn't match", []() {
        Endpoint endpoint("/plain_path");

        expect(endpoint.matches("/non_matching")).toBe(false);
    });

    it("returns true when path matches", []() {
        expect(Endpoint("/plain_path").matches("/plain_path")).toBe(true);
        expect(Endpoint("/plain_path/:parameter").matches("/plain_pa/parameter")).toBe(false);
        expect(Endpoint("/plain_path/:parameter").matches("/plain_path/value")).toBe(true);
        expect(Endpoint("/leve1/:parameter1/:parameter2").matches("/leve1/value1/3.7.3")).toBe(true);
        expect(Endpoint("/leve1/:parameter1/:parameter2").matches("/leve1/value1")).toBe(false);
    });

    it("returns empty match when path with parameters doesn't match", []() {
        Endpoint endpoint("/plain_path/:parameter");
        struct HttpParameterMap match;

        match = endpoint.match("/plain_pa/parameter");

        expect(match.has("parameter")).toBe(false);
    });

    it("returns match when path with one parameter match", []() {
        Endpoint endpoint("/plain_path/:parameter");
        struct HttpParameterMap match;

        match = endpoint.match("/plain_path/value");

        expect(match.get("parameter")).toBe("value");
    });

    it("returns match when path with one parameter match alphanumeric", []() {
        Endpoint endpoint("/plain_path/:parameter");
        struct HttpParameterMap match;

        match = endpoint.match("/plain_path/3.7.3");

        expect(match.get("parameter")).toBe("3.7.3");
    });

    it("returns match when path with many parameter matches", []() {
        Endpoint endpoint("/leve1/:parameter1/:parameter2");
        struct HttpParameterMap match;

        match = endpoint.match("/leve1/value1/3.7.3");

        expect(match.get("parameter1")).toBe("value1");
        expect(match.get("parameter2")).toBe("3.7.3");
    });

    it("compares two endpoints alphabetically by matching regex", []() {
        Endpoint endpoint1("/plain_path/:parameter");
        Endpoint endpoint2("/plain_path/:parameter/:parameter2");

        expect(endpoint1 < endpoint2).toBe(true);
    });
});
